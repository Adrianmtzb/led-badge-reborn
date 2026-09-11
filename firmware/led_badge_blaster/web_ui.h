#pragma once
#include <Arduino.h>

// Página servida por el propio dispositivo. Se manda tal cual, sin plantillas:
// todo lo dinámico se pide por /api/*. Sin fuentes ni scripts externos, porque
// en modo portal no hay salida a internet.

static const char INDEX_HTML[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="color-scheme" content="dark light">
<title>LED Badge Blaster</title>
<style>
:root{
  --ink:#0b0d12; --panel:#141821; --sunk:#0e1118; --line:#242a36;
  --text:#e9ebf2; --dim:#8d94a8; --warn:#ffb454;
  --live:#f05000; --live-ink:#000;
  --r:14px;
}
@media(prefers-color-scheme:light){
  :root{--ink:#f4f5f8; --panel:#fff; --sunk:#eceef3; --line:#dcdfe8;
        --text:#141821; --dim:#5f6779; --warn:#9a5b00}
}
*{box-sizing:border-box}
html{-webkit-text-size-adjust:100%}
body,button,summary{touch-action:manipulation}
button{-webkit-tap-highlight-color:transparent}
body{
  margin:0 auto; max-width:44rem; padding:0 1rem env(safe-area-inset-bottom);
  background:var(--ink); color:var(--text);
  font:16px/1.5 system-ui,-apple-system,"Segoe UI",sans-serif;
  -webkit-font-smoothing:antialiased;
}
button{font:inherit;color:inherit;cursor:pointer}
:focus-visible{outline:2px solid var(--text);outline-offset:2px}

/* --- lectura en vivo: lo que el badge va a emitir --- */
#live{
  background:var(--live); color:var(--live-ink);
  border-radius:0 0 var(--r) var(--r);
  padding:1.3rem 1rem 1.05rem; margin:0 -1rem 1.5rem;
  transition:background .25s ease;
}
#live h1{margin:0;font-size:1.65rem;line-height:1.08;font-weight:640;letter-spacing:-.022em}
#live p{margin:.3rem 0 0;font-size:.86rem;opacity:.78}
#live .tags{display:flex;gap:.9rem;margin-top:.6rem;font-size:.74rem;opacity:.68;
  font-family:ui-monospace,SFMono-Regular,Menlo,monospace}
#live small{display:block;margin-top:.55rem;font-size:.78rem;opacity:.72}

/* --- categorías --- */
#tabs{display:flex;gap:.4rem;overflow-x:auto;scrollbar-width:none;
  margin:0 -1rem 0;padding:0 1rem .2rem}
#tabs::-webkit-scrollbar{display:none}
#tabs button{
  flex:none;padding:.42rem .8rem;border:1px solid var(--line);border-radius:999px;
  background:var(--panel);font-size:.88rem;white-space:nowrap;
}
#tabs button[aria-selected=true]{background:var(--text);border-color:var(--text);color:var(--ink);font-weight:600}
#blurb{margin:.8rem 0 1rem;font-size:.85rem;color:var(--dim);max-width:52ch}

/* --- rejilla de efectos: el color ocupa casi todo el botón --- */
#grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(6.2rem,1fr));gap:.6rem}
#grid button{
  display:block;width:100%;padding:0;border:1px solid var(--line);border-radius:12px;
  background:var(--panel);overflow:hidden;text-align:left;
  transition:transform .12s ease,border-color .12s ease;
}
#grid button:active{transform:scale(.97)}
#grid button .sw{display:block;height:3.6rem;border-bottom:1px solid var(--line)}
#grid button .cap{display:block;padding:.42rem .55rem .5rem}
#grid button .nm{font-size:.8rem;line-height:1.25;overflow:hidden;
  display:-webkit-box;-webkit-line-clamp:2;-webkit-box-orient:vertical}
#grid button .hx{display:block;margin-top:.1rem;font-size:.65rem;color:var(--dim);
  font-family:ui-monospace,SFMono-Regular,Menlo,monospace}
#grid button[aria-pressed=true]{border-color:var(--text);box-shadow:inset 0 0 0 1px var(--text)}
#grid button[aria-pressed=true] .nm{font-weight:640}

/* --- ajustes, plegados: no es lo que vienes a hacer --- */
details{margin:2rem 0 0;border-top:1px solid var(--line);padding-top:1rem}
summary{cursor:pointer;font-size:.9rem;color:var(--dim);list-style:none}
summary::-webkit-details-marker{display:none}
summary::before{content:"+ ";font-family:ui-monospace,monospace}
details[open] summary::before{content:"– "}
details[open] summary{margin-bottom:1rem;color:var(--text)}
label{display:block;font-size:.8rem;color:var(--dim);margin:0 0 .3rem}
input{width:100%;padding:.6rem .7rem;border:1px solid var(--line);border-radius:10px;
  background:var(--sunk);color:var(--text);font:inherit;font-size:1rem;margin-bottom:.9rem}
#nets{display:flex;flex-direction:column;gap:.3rem;max-height:13rem;overflow-y:auto;margin-bottom:.9rem}
#nets button{display:flex;align-items:center;justify-content:space-between;gap:.75rem;
  width:100%;padding:.55rem .7rem;border:1px solid var(--line);border-radius:10px;
  background:var(--panel);text-align:left}
#nets .ss{overflow:hidden;text-overflow:ellipsis;white-space:nowrap;font-size:.9rem}
#nets .meta{display:flex;align-items:center;gap:.5rem;flex:none;color:var(--dim);font-size:.72rem}
.bars{display:flex;align-items:flex-end;gap:2px;height:12px}
.bars i{width:3px;border-radius:1px;background:var(--line)}
.bars i.on{background:var(--text)}
.bars i:nth-child(1){height:25%}.bars i:nth-child(2){height:50%}
.bars i:nth-child(3){height:75%}.bars i:nth-child(4){height:100%}
.row{display:flex;gap:.5rem;flex-wrap:wrap}
.row button{padding:.6rem 1rem;border-radius:10px;border:1px solid var(--line);background:var(--panel)}
.row button.primary{background:var(--text);border-color:var(--text);color:var(--ink);font-weight:600}
#msg{font-size:.85rem;color:var(--dim);margin-top:.8rem;min-height:1.3em}
#msg.warn{color:var(--warn)}
footer{margin:2rem 0;padding-top:1rem;border-top:1px solid var(--line);
  font-size:.75rem;line-height:1.55;color:var(--dim)}
footer b{color:var(--text);font-weight:600}
footer p{margin:0 0 .7rem}
#ver{font-family:ui-monospace,SFMono-Regular,Menlo,monospace;font-size:.72rem}
#ver b{font-weight:600}
@media(prefers-reduced-motion:reduce){*{transition:none!important}}
</style>
</head>
<body>

<div id="live">
  <h1 id="live-name">Conectando…</h1>
  <p id="live-note"></p>
  <div class="tags"><span id="live-id"></span><span id="live-hex"></span></div>
  <small id="live-net"></small>
</div>

<main>
  <nav id="tabs" aria-label="Categorías"></nav>
  <p id="blurb"></p>
  <div id="grid"></div>
</main>

<details>
  <summary>Red y ajustes</summary>
  <form id="wf">
    <label for="ssid">Red WiFi</label>
    <input id="ssid" autocomplete="off" maxlength="32" required>
    <div id="nets"></div>
    <label for="pass">Contraseña</label>
    <input id="pass" type="password" autocomplete="new-password" maxlength="63">
    <div class="row">
      <button class="primary" type="submit">Guardar y conectar</button>
      <button type="button" id="scan">Buscar redes</button>
      <button type="button" id="forget">Olvidar red</button>
    </div>
  </form>
  <div id="msg"></div>
</details>

<footer>
  <p id="ver">Firmware <b>—</b></p>
  <p><b>Herramienta independiente, compatible con badges PixMob.</b> No está
  afiliada ni respaldada por PixMob / Eski Inc. Úsala solo con dispositivos
  propios o que tengas permiso para probar. No la uses en eventos en directo ni
  para interferir con espectáculos, equipos de recinto o dispositivos ajenos.</p>
</footer>

<script>
const H={'X-Requested-With':'led-badge'};
const $=s=>document.querySelector(s);
let cats=[],cmds=[],st={},tab='';

const j=async(u,o)=>{const r=await fetch(u,o);if(!r.ok)throw new Error(r.status);return r.json()};
const msg=(t,warn)=>{const m=$('#msg');m.textContent=t;m.classList.toggle('warn',!!warn)};

// Sobre un color arbitrario, el texto se elige por luminancia: si no, los
// amarillos del catálogo quedan ilegibles con tinta blanca.
function inkFor(hex){
  const n=parseInt(hex.slice(1),16);
  const f=c=>{c/=255;return c<=.03928?c/12.92:Math.pow((c+.055)/1.055,2.4)};
  const L=.2126*f(n>>16&255)+.7152*f(n>>8&255)+.0722*f(n&255);
  return L>.36?'#000':'#fff';
}

function paintLive(){
  const c=cmds.find(c=>c.i===st.index);
  const hex=c?c.color:'#333333';
  document.documentElement.style.setProperty('--live',hex);
  document.documentElement.style.setProperty('--live-ink',inkFor(hex));
  $('#live-name').textContent=c?c.name:'Sin efecto';
  $('#live-note').textContent=c?(c.note||'Toca un color para emitirlo'):'';
  $('#live-id').textContent=c?c.id:'';
  $('#live-hex').textContent=c?c.color:'';
  const n=st.net||{};
  $('#live-net').textContent=n.mode==='STA'
    ?'En la red '+n.ssid+', responde en '+n.ip
    :'Portal propio '+n.ssid+', responde en '+n.ip;
}

function drawTabs(){
  const nav=$('#tabs');nav.textContent='';
  for(const c of cats){
    const b=document.createElement('button');
    b.textContent=c.label;
    b.setAttribute('aria-selected',c.key===tab);
    b.onclick=()=>{tab=c.key;drawTabs();drawGrid()};
    nav.appendChild(b);
  }
  const c=cats.find(c=>c.key===tab);
  $('#blurb').textContent=c?c.blurb:'';
}

function drawGrid(){
  const g=$('#grid');g.textContent='';
  for(const c of cmds){
    if(c.mode!==tab)continue;
    const b=document.createElement('button');
    b.type='button';
    b.setAttribute('aria-pressed',c.i===st.index);
    b.title=c.note||c.name;

    const sw=document.createElement('span');
    sw.className='sw';sw.style.background=c.color;

    const cap=document.createElement('span');
    cap.className='cap';
    const nm=document.createElement('span');
    nm.className='nm';nm.textContent=c.name;
    const hx=document.createElement('span');
    hx.className='hx';hx.textContent=c.color;
    cap.append(nm,hx);

    b.append(sw,cap);
    b.onclick=()=>send(c.i);
    g.appendChild(b);
  }
}

// El dispositivo atiende una petición a la vez, así que tocar rápido encolaba
// un POST y un GET de estado por toque y el panel se quedaba atrás. Ahora la
// selección se pinta al instante y, mientras hay un envío en vuelo, los toques
// siguientes se resumen en el último: lo que importa es el color que acabas de
// tocar, no emitir los seis intermedios.
let sending=false,pending=null;

async function send(i){
  st.index=i;paintLive();drawGrid();

  if(sending){pending=i;return}
  sending=true;
  try{
    let idx=i;
    for(;;){
      await j('/api/send?index='+idx,{method:'POST',headers:H});
      if(pending===null)break;
      idx=pending;pending=null;
    }
    msg('');
  }catch(e){
    msg('No se pudo emitir. Comprueba la conexión con el dispositivo.',1);
    await refresh().catch(()=>{});   // el estado local puede haberse ido
  }finally{sending=false;pending=null}
}

async function refresh(){
  st=await j('/api/state');
  drawVersion();
  paintLive();
  drawGrid();
}

// La versión sale de /api/state, así que es la que corre de verdad en la placa.
// Sirve para comprobar de un vistazo si una reinstalación entró.
function drawVersion(){
  const el=$('#ver');
  el.textContent='Firmware ';
  const b=document.createElement('b');
  b.textContent=st.version?'v'+st.version:'desconocida';
  el.appendChild(b);
}

const level=r=>r>=-55?4:r>=-65?3:r>=-75?2:1;

function drawNets(nets){
  const box=$('#nets');box.textContent='';

  // Un mismo SSID puede aparecer varias veces (repetidores): nos quedamos con
  // la antena que mejor se oye. Las de nombre vacío son redes ocultas.
  const best=new Map();
  for(const n of nets){
    if(!n.ssid)continue;
    const prev=best.get(n.ssid);
    if(!prev||n.rssi>prev.rssi)best.set(n.ssid,n);
  }
  const list=[...best.values()].sort((a,b)=>b.rssi-a.rssi);

  for(const n of list){
    const b=document.createElement('button');
    b.type='button';

    const ss=document.createElement('span');
    ss.className='ss';
    ss.textContent=n.ssid;              // textContent: el SSID es texto ajeno

    const meta=document.createElement('span');
    meta.className='meta';
    if(!n.lock){const o=document.createElement('span');o.textContent='abierta';meta.appendChild(o)}

    const bars=document.createElement('span');
    bars.className='bars';
    const lv=level(n.rssi);
    for(let k=1;k<=4;k++){const i=document.createElement('i');if(k<=lv)i.className='on';bars.appendChild(i)}
    meta.appendChild(bars);

    b.append(ss,meta);
    b.onclick=()=>{$('#ssid').value=n.ssid;$('#pass').focus();msg('Red elegida: '+n.ssid)};
    box.appendChild(b);
  }
  return list.length;
}

$('#scan').onclick=async()=>{
  msg('Buscando redes…');
  try{
    const n=drawNets(await j('/api/scan'));
    msg(n?'Toca una red para elegirla.':'No se vio ninguna red.');
  }catch(e){msg('El escaneo falló. Inténtalo otra vez.',1)}
};

$('#wf').onsubmit=async e=>{
  e.preventDefault();
  const body=new URLSearchParams({ssid:$('#ssid').value,pass:$('#pass').value});
  msg('Guardando…');
  try{
    await fetch('/api/wifi',{method:'POST',headers:{...H,'Content-Type':'application/x-www-form-urlencoded'},body});
    msg('Guardado. El dispositivo se reinicia y se une a esa red; reconéctate a ella para seguir.');
  }catch(e){msg('No se pudo guardar. Revisa la contraseña e inténtalo otra vez.',1)}
};

$('#forget').onclick=async()=>{
  if(!confirm('¿Borrar las credenciales y volver al portal?'))return;
  await fetch('/api/forget',{method:'POST',headers:H});
  msg('Credenciales borradas. Reiniciando en modo portal.');
};

(async()=>{
  try{
    const d=await j('/api/commands');
    cats=d.categories;cmds=d.commands;
    tab=(cats[0]||{}).key||'';
    drawTabs();await refresh();
  }catch(e){$('#live-name').textContent='Sin respuesta';$('#live-note').textContent='Recarga la página o vuelve a conectarte a la red del dispositivo.'}
})();
</script>
</body>
</html>
)HTML";
