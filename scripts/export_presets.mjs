/*
 * Reexporta data/presets/*.json desde el generador web.
 *
 * El generador corre en el navegador, así que la exportación se hace
 * conduciendo un Chrome real con Playwright: se abre la pestaña de presets y
 * se pulsa el botón .json de cada categoría. Lo que queda en data/presets/ es
 * la descarga literal de la herramienta, sin pasos manuales que documentar.
 *
 *   npm i -D playwright && npx playwright install chromium
 *   node scripts/export_presets.mjs
 *
 * Los nombres de fichero son los de las categorías; el generador los prefija,
 * así que aquí se les quita el prefijo al guardar.
 */

import { chromium } from 'playwright';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const OUT = path.join(ROOT, 'data', 'presets');
const URL_GEN = process.env.GENERATOR_URL || 'https://adrianmtzb.github.io/led-badge-encoder/';

fs.mkdirSync(OUT, { recursive: true });

const browser = await chromium.launch();
const ctx = await browser.newContext({ acceptDownloads: true });
const page = await ctx.newPage();
page.on('pageerror', e => console.error('error en la página:', e.message));

// El parámetro rompe la caché: si no, una actualización del generador puede
// tardar en verse y se exportarían nombres viejos.
await page.goto(`${URL_GEN}?t=${Date.now()}`, { waitUntil: 'networkidle' });
await page.click('#tab-presets');
await page.waitForSelector('#preset-categories .preset-cat');

const cats = await page.$$('#preset-categories .preset-cat');
if (!cats.length) {
  await browser.close();
  throw new Error('el generador no mostró ninguna categoría');
}

for (const cat of cats) {
  const title = (await cat.$eval('h2', n => n.textContent)).trim();
  const [dl] = await Promise.all([
    page.waitForEvent('download'),
    cat.$('header .actions button:has-text(".json")').then(b => b.click()),
  ]);
  const name = dl.suggestedFilename().replace(/^[a-z]+-/, '');
  await dl.saveAs(path.join(OUT, name));
  console.log(`${title} -> data/presets/${name}`);
}

await browser.close();
console.log(`\n${cats.length} categorías exportadas. Ahora: make catalog`);
