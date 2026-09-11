#pragma once
#include <Arduino.h>

// Página servida por el propio dispositivo. Se manda tal cual, sin plantillas:
// todo lo dinámico se pide por /api/*.

static const char INDEX_HTML[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>LED Badge</title>
<style>
:root{color-scheme:light dark;--bg:#fff;--fg:#111;--mut:#666;--line:#d8d8d8;--acc:#0a84ff}
@media(prefers-color-scheme:dark){:root{--bg:#161618;--fg:#f2f2f2;--mut:#9a9a9a;--line:#333;--acc:#4c9fff}}
*{box-sizing:border-box}
body{margin:0;padding:1.25rem;font:16px/1.5 system-ui,-apple-system,sans-serif;background:var(--bg);color:var(--fg);max-width:44rem;margin-inline:auto}
h1{font-size:1.35rem;margin:0}
h2{font-size:1rem;margin:0 0 .75rem;text-transform:uppercase;letter-spacing:.05em;color:var(--mut)}
header{display:flex;justify-content:space-between;align-items:baseline;gap:1rem;flex-wrap:wrap;margin-bottom:1.5rem}
#net{color:var(--mut);font-size:.85rem;text-align:right}
.card{border:1px solid var(--line);border-radius:12px;padding:1rem;margin-bottom:1.25rem}
nav{display:flex;gap:.5rem;margin-bottom:1rem}
nav button{flex:1;padding:.6rem;border:1px solid var(--line);background:transparent;color:var(--fg);border-radius:8px;cursor:pointer;font-size:.9rem}
nav button[aria-selected=true]{background:var(--acc);border-color:var(--acc);color:#fff}
#grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(4rem,1fr));gap:.5rem}
#grid button{padding:.75rem 0;border:1px solid var(--line);background:transparent;color:var(--fg);border-radius:8px;cursor:pointer;font-variant-numeric:tabular-nums}
#grid button:hover{border-color:var(--acc)}
#grid button.on{background:var(--acc);border-color:var(--acc);color:#fff}
label{display:block;font-size:.85rem;color:var(--mut);margin-bottom:.25rem}
input{width:100%;padding:.55rem;border:1px solid var(--line);border-radius:8px;background:transparent;color:var(--fg);margin-bottom:.75rem}
#nets{display:flex;flex-direction:column;gap:.25rem;max-height:14rem;overflow-y:auto;margin-bottom:.75rem}
#nets button{display:flex;align-items:center;justify-content:space-between;gap:.75rem;width:100%;padding:.5rem .6rem;border:1px solid var(--line);border-radius:8px;background:transparent;color:var(--fg);cursor:pointer;text-align:left;font:inherit}
#nets button:hover{border-color:var(--acc)}
#nets .ss{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
#nets .meta{display:flex;align-items:center;gap:.5rem;flex:none;color:var(--mut);font-size:.75rem}
.bars{display:flex;align-items:flex-end;gap:2px;height:12px}
.bars i{width:3px;border-radius:1px;background:var(--line)}
.bars i.on{background:var(--fg)}
.bars i:nth-child(1){height:25%}
.bars i:nth-child(2){height:50%}
.bars i:nth-child(3){height:75%}
.bars i:nth-child(4){height:100%}
.row{display:flex;gap:.5rem;flex-wrap:wrap}
.row button{padding:.55rem 1rem;border-radius:8px;border:1px solid var(--line);background:transparent;color:var(--fg);cursor:pointer}
button.primary{background:var(--acc);border-color:var(--acc);color:#fff}
#msg{font-size:.85rem;margin-top:.75rem;min-height:1.2em}
</style>
</head>
<body>
<header>
  <h1>LED Badge</h1>
  <div id="net">cargando…</div>
</header>

<section class="card">
  <h2>Efectos</h2>
  <nav id="tabs"></nav>
  <div id="grid"></div>
</section>

<section class="card">
  <h2>WiFi</h2>
  <form id="wf">
    <label for="ssid">Red</label>
    <input id="ssid" autocomplete="off" maxlength="32" required>
    <div id="nets"></div>
    <label for="pass">Contraseña</label>
    <input id="pass" type="password" autocomplete="new-password" maxlength="63">
    <div class="row">
      <button class="primary" type="submit">Guardar y conectar</button>
      <button type="button" id="scan">Buscar redes</button>
      <button type="button" id="forget">Olvidar</button>
    </div>
  </form>
  <div id="msg"></div>
</section>

<script>
const H={'X-Requested-With':'led-badge'};
const $=s=>document.querySelector(s);
let cmds=[],st={},tab='PULSO';

async function j(u,o){const r=await fetch(u,o);if(!r.ok)throw new Error(r.status);return r.json()}
function msg(t){$('#msg').textContent=t}

function drawNet(){
  const n=st.net||{};
  $('#net').textContent=(n.mode==='STA'?'Conectado a '+n.ssid:'Portal '+n.ssid)+' · '+n.ip;
}

function drawTabs(){
  const nav=$('#tabs');nav.textContent='';
  for(const m of ['SPECIAL','PULSO','FADE']){
    const b=document.createElement('button');
    b.textContent=m;
    b.setAttribute('aria-selected',m===tab);
    b.onclick=()=>{tab=m;drawTabs();drawGrid()};
    nav.appendChild(b);
  }
}

function drawGrid(){
  const g=$('#grid');g.textContent='';
  for(const c of cmds.filter(c=>c.mode===tab)){
    const b=document.createElement('button');
    b.textContent=c.label.replace(/^[A-Z]+_/,'');
    b.title=c.label;
    if(c.i===st.index)b.className='on';
    b.onclick=()=>send(c.i);
    g.appendChild(b);
  }
}

async function send(i){
  try{await j('/api/send?index='+i,{method:'POST',headers:H});msg('');await refresh()}
  catch(e){msg('No se pudo enviar')}
}

async function refresh(){st=await j('/api/state');drawNet();drawGrid()}

function level(r){return r>=-55?4:r>=-65?3:r>=-75?2:1}

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
  msg('Buscando…');
  try{
    const n=drawNets(await j('/api/scan'));
    msg(n?'Toca una red para elegirla.':'No se vio ninguna red.');
  }catch(e){msg('Fallo el escaneo')}
};

$('#wf').onsubmit=async e=>{
  e.preventDefault();
  const body=new URLSearchParams({ssid:$('#ssid').value,pass:$('#pass').value});
  msg('Guardando…');
  try{
    await fetch('/api/wifi',{method:'POST',headers:{...H,'Content-Type':'application/x-www-form-urlencoded'},body});
    msg('Guardado. El dispositivo se reinicia y se une a la red; reconéctate a ella para seguir usándolo.');
  }catch(e){msg('No se pudo guardar')}
};

$('#forget').onclick=async()=>{
  if(!confirm('¿Borrar las credenciales y volver al portal?'))return;
  await fetch('/api/forget',{method:'POST',headers:H});
  msg('Credenciales borradas. Reiniciando en modo portal.');
};

(async()=>{
  try{cmds=await j('/api/commands');drawTabs();await refresh()}
  catch(e){msg('No se pudo contactar con el dispositivo')}
})();
</script>
</body>
</html>
)HTML";
