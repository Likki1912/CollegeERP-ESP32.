/*
  College ERP - ESP32 DevKit V1
  Demo web application hosted by the ESP32 in Access Point mode.

  Wi-Fi:
    SSID: CollegeERP-ESP32
    Password: college123
  Open: http://192.168.4.1

  Demo credentials:
    Admin:   admin / admin123
    Student: student / student123

  IMPORTANT: This is a learning prototype. Authentication and records are
  stored in the page's JavaScript only; do not use for real student data.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "CollegeERP-ESP32";
const char* AP_PASSWORD = "college123";

WebServer server(80);

const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>College ERP | ESP32</title>
<style>
:root{--navy:#14213d;--blue:#2563eb;--light:#f3f6fb;--text:#172033;--muted:#64748b;--white:#fff;--green:#16845b;--border:#e2e8f0}
*{box-sizing:border-box}body{margin:0;font-family:Arial,Helvetica,sans-serif;background:var(--light);color:var(--text)}
header{background:var(--navy);color:#fff;padding:18px 22px;display:flex;justify-content:space-between;align-items:center;gap:12px;flex-wrap:wrap}
header h1{font-size:20px;margin:0}header small{color:#dbeafe}
main{max-width:1100px;margin:24px auto;padding:0 16px}
.card{background:#fff;border:1px solid var(--border);border-radius:14px;padding:20px;box-shadow:0 5px 18px #0f172a0a;margin-bottom:16px}
h2{margin-top:0;font-size:22px}h3{margin-top:0}
input,select,button{font:inherit;border-radius:8px;padding:11px 12px;border:1px solid #cbd5e1}
input,select{width:100%;margin:6px 0 12px;background:#fff}
button{background:var(--blue);color:#fff;border:0;cursor:pointer;font-weight:600}
button.secondary{background:#e8eefb;color:#173b7a}button:hover{filter:brightness(.96)}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:14px}
.stat{padding:18px;border-radius:12px;background:#eef4ff;border:1px solid #dbeafe}
.stat strong{display:block;font-size:25px;color:var(--blue);margin-top:8px}
.nav{display:flex;gap:8px;flex-wrap:wrap;margin-bottom:16px}
.nav button{background:#e8eefb;color:#173b7a}
.nav button.active{background:var(--blue);color:#fff}
table{width:100%;border-collapse:collapse;background:#fff}th,td{text-align:left;padding:11px;border-bottom:1px solid var(--border)}th{background:#f8fafc}
.tablewrap{overflow:auto}.muted{color:var(--muted)}.hidden{display:none}.notice{padding:12px;border-radius:8px;background:#ecfdf5;color:#065f46;margin:10px 0}
.error{color:#b91c1c;margin:8px 0}.pill{display:inline-block;background:#dcfce7;color:#166534;border-radius:20px;padding:5px 9px;font-size:12px}
footer{text-align:center;color:var(--muted);font-size:12px;padding:18px}
@media(max-width:500px){header{align-items:flex-start}main{margin-top:14px}.card{padding:15px}}
</style>
</head>
<body>
<header><div><h1>🎓 College ERP</h1><small>ESP32 Wi-Fi Web Application</small></div><div id="userBadge">Demo Portal</div></header>
<main>
<section id="login" class="card">
<h2>Welcome to College ERP</h2><p class="muted">Sign in to access the demo dashboard.</p>
<div class="grid">
<div><label for="username">Username</label><input id="username" placeholder="admin or student" autocomplete="username"></div>
<div><label for="password">Password</label><input id="password" type="password" placeholder="Enter password" autocomplete="current-password"></div>
</div>
<button onclick="login()">Login</button><div id="loginError" class="error"></div>
<p class="muted">Demo accounts: <b>admin / admin123</b> or <b>student / student123</b></p>
</section>
<section id="app" class="hidden">
<div class="nav" id="nav"></div>
<div id="content"></div>
</section>
</main>
<footer>College ERP prototype • Hosted locally by ESP32 • Demo data only</footer>
<script>
let currentRole='', currentUser='';
const students=[
 {id:'CSE001',name:'Aarav Kumar',branch:'CSE',year:'2nd Year',attendance:'92%',marks:'86%',fees:'Paid'},
 {id:'ECE002',name:'Ananya Rao',branch:'ECE',year:'2nd Year',attendance:'88%',marks:'91%',fees:'Pending'},
 {id:'EEE003',name:'Rahul Varma',branch:'EEE',year:'3rd Year',attendance:'95%',marks:'79%',fees:'Paid'},
 {id:'AIDS004',name:'Sana Reddy',branch:'AIDS',year:'1st Year',attendance:'90%',marks:'88%',fees:'Paid'},
 {id:'CYB005',name:'Kiran Das',branch:'Cyber Security',year:'3rd Year',attendance:'84%',marks:'82%',fees:'Pending'},
 {id:'MEC006',name:'Meera Singh',branch:'MECH',year:'2nd Year',attendance:'89%',marks:'80%',fees:'Paid'}
];
const pages=['Dashboard','Students','Attendance','Marks','Fees','Timetable'];
function login(){
 const u=document.getElementById('username').value.trim(), p=document.getElementById('password').value;
 if((u==='admin'&&p==='admin123')||(u==='student'&&p==='student123')){
 currentRole=u==='admin'?'Admin':'Student';currentUser=u;
 document.getElementById('login').classList.add('hidden');document.getElementById('app').classList.remove('hidden');
 document.getElementById('userBadge').innerHTML=currentRole+' • '+u;
 buildNav();showPage('Dashboard');
 }else document.getElementById('loginError').textContent='Invalid login. Use one of the demo accounts.';
}
function buildNav(){
 const nav=document.getElementById('nav');
 nav.innerHTML=pages.map(p=>`<button id="nav-${p}" onclick="showPage('${p}')">${p}</button>`).join('')+
 '<button class="secondary" onclick="logout()">Logout</button>';
}
function logout(){currentRole='';currentUser='';document.getElementById('app').classList.add('hidden');document.getElementById('login').classList.remove('hidden');document.getElementById('password').value='';document.getElementById('loginError').textContent='';document.getElementById('userBadge').textContent='Demo Portal'}
function showPage(page){
 pages.forEach(p=>{const b=document.getElementById('nav-'+p);if(b)b.classList.toggle('active',p===page)});
 const c=document.getElementById('content');
 if(page==='Dashboard'){
 c.innerHTML=`<div class="card"><h2>${currentRole} Dashboard</h2><p class="muted">Welcome, ${currentUser}. This is a local demo portal.</p><div class="grid">
 <div class="stat">Registered Students<strong>${students.length}</strong></div><div class="stat">Departments<strong>6</strong></div><div class="stat">Attendance Average<strong>90%</strong></div><div class="stat">Academic Year<strong>2026–27</strong></div></div></div>
 <div class="card"><h3>Available Modules</h3><p>Use the navigation buttons to view student records, attendance, marks, fees and timetable.</p><span class="pill">Prototype mode</span></div>`;
 }else if(page==='Students'){
 let rows=students.filter(s=>currentRole==='Admin'||s.id==='ECE002');
 c.innerHTML=`<div class="card"><h2>Student Directory</h2><div class="tablewrap"><table><thead><tr><th>ID</th><th>Name</th><th>Branch</th><th>Year</th></tr></thead><tbody>${rows.map(s=>`<tr><td>${s.id}</td><td>${s.name}</td><td>${s.branch}</td><td>${s.year}</td></tr>`).join('')}</tbody></table></div></div>`;
 }else if(page==='Attendance'||page==='Marks'||page==='Fees'){
 const field=page==='Attendance'?'attendance':page==='Marks'?'marks':'fees';
 const title=page==='Attendance'?'Attendance Records':page==='Marks'?'Internal Marks':'Fee Status';
 let rows=students.filter(s=>currentRole==='Admin'||s.id==='ECE002');
 c.innerHTML=`<div class="card"><h2>${title}</h2><div class="tablewrap"><table><thead><tr><th>Student</th><th>ID</th><th>${page==='Fees'?'Status':page==='Marks'?'Score':'Attendance'}</th></tr></thead><tbody>${rows.map(s=>`<tr><td>${s.name}</td><td>${s.id}</td><td>${s[field]}</td></tr>`).join('')}</tbody></table></div></div>`;
 }else if(page==='Timetable'){
 c.innerHTML=`<div class="card"><h2>Sample Timetable</h2><div class="tablewrap"><table><thead><tr><th>Day</th><th>09:00–10:00</th><th>10:00–11:00</th><th>11:15–12:15</th></tr></thead><tbody><tr><td>Monday</td><td>Mathematics</td><td>Digital Systems</td><td>Programming</td></tr><tr><td>Tuesday</td><td>Electronics</td><td>Data Structures</td><td>Lab</td></tr><tr><td>Wednesday</td><td>Physics</td><td>Communication</td><td>Project</td></tr><tr><td>Thursday</td><td>Programming</td><td>Mathematics</td><td>Lab</td></tr><tr><td>Friday</td><td>Digital Systems</td><td>Electronics</td><td>Seminar</td></tr></tbody></table></div></div>`;
 }
}
</script>
</body>
</html>
)HTML";

void handleRoot() {
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void handleNotFound() {
  server.send(404, "text/plain", "404 - Page not found. Open http://192.168.4.1");
}

void setup() {
  Serial.begin(115200);
  delay(300);
  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(AP_SSID, AP_PASSWORD);
  if (ok) {
    Serial.println();
    Serial.println("College ERP Wi-Fi started.");
    Serial.print("Wi-Fi name: "); Serial.println(AP_SSID);
    Serial.print("Wi-Fi password: "); Serial.println(AP_PASSWORD);
    Serial.print("Open in browser: http://"); Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Wi-Fi access point.");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Web server started on port 80.");
}

void loop() {
  server.handleClient();
}
