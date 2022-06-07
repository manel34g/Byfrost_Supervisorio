var mqtt;
var reconnectTimeout = 2000;
var host = "test.mosquitto.org";
var port = 8080;
var onStt = new Boolean(false);
var supStt = new Boolean(false);
var ext = new Boolean(false)
function onConnect() {
  console.log("Conectado");
  atualizeStt("sup_stt","on");
  mqtt.subscribe("byf/hem");
  mqtt.subscribe("byf/hem/stt");
  mqttLastMsg("Supervisório conectado","success");
}
function onError(message) {
  console.log("Falha: " + message.errorCode + " " + message.errorMessage);
  setTimeout(MQTTConnect, reconnectTimeout);
  atualizeStt("sup_stt","off");
  mqttLastMsg("Supervisório desconectado","danger");
}
function onMessageArrived(msg) {
// Mensagem recebida
  console.log("Mensagem: " + msg.destinationName + "=" + msg.payloadString);
  if (msg.payloadString.indexOf("received") != -1) {
    var str = msg.payloadString.substring(9);
    mqttLastMsg("Mensagem recebida: "+str,"secondary");
    ext==false;
  }else if (msg.payloadString.indexOf("end") != -1) {
    if(ext==false){
      mqttLastMsg("Ordem executada","warning");
      ext==true;
    }
  }else if (msg.payloadString.indexOf("prepared") != -1) {
    if(ext==false){
      mqttLastMsg("Heimdall preparado","warning");
      ext==true;
    }
  } 
  if(msg.destinationName=="byf/hem/stt"){
    onStt=true;
    clearTimeout(waitTime);
    checkOn();
    onStt=false;
  }
}
function MQTTConnect() {
// Conecta-se ao Broker
  console.log("Conectando " + host + " " + port);
  mqtt = new Paho.MQTT.Client(host, port, "IeCJSClient" + parseInt(Math.random() * 1e5));
  var options = { timeout: 10,
                keepAliveInterval: 10,
                onSuccess: onConnect,
                onFailure: onError
              };
  mqtt.onMessageArrived = onMessageArrived;
  mqtt.onConnectionLost = onError;
  mqtt.connect(options);
}
function sendToMqtt(msg){
  if (msg=="0") {

  }else{
    if (msg[0]=="*") {
      var nMsg = msg.slice(1,msg.length);

      console.log(nMsg);

      message = new Paho.MQTT.Message("add_"+nMsg);
      message.destinationName = "byf/hem";
      mqtt.send(message);

      mqttLastMsg("Adicionando a : "+nMsg,"info");
    }else{
      message = new Paho.MQTT.Message("add_"+msg);
      message.destinationName = "byf/hem";
      mqtt.send(message);

      mqttLastMsg("Adicionando a : "+msg,"info");
    }
    
    
    
  }
}


//Aparição da Sidebar 
var btn_sidebar = new Boolean(false);
var btn_sobre = 0;
var btn_heimdall = 0;

function appearSidebar(id){
  console.log("foi");
  btn_sidebar= !btn_sidebar;

  var Sidebar = document.getElementById("sidebar");
  var Menu = document.getElementById("menu");
  var Sidebar_sobre = document.getElementById("sidebar-sobre");
  var Sidebar_heimdall = document.getElementById("sidebar-heimdall");
  if (btn_sidebar==1) {
    Sidebar.style.marginLeft="0vw";
    Menu.style.width="65vw";

  }if (id =="heimdall") {
    Sidebar_sobre.style.marginTop="-97vh";
    btn_sobre=1;
    btn_heimdall++;
  }else if (id =="sobre") {
    Sidebar_sobre.style.marginTop="0";
    btn_heimdall=1;
    btn_sobre++;
  }if (btn_sobre>=3||btn_heimdall>=3) {
    Sidebar.style.marginLeft="-35vw";
    Menu.style.width="100vw";
    btn_heimdall=1;
    btn_sobre=1;
  }
}

var selected=0;
var op ="";
var lastOp="";
var nMax=0;
function opSelect(id){
  op = id;
  if (lastOp=="") {
    lastOp=op;
  }
  if (op == "cancel") {
    Disable(); 

    op ="";
    lastOp ="";

    nMax=0;
    selected=0;
  }else if (op=="add"&&lastOp==op) {

    nMax=1;

    console.log("Adicionando");
    var cll=document.querySelectorAll("div#storage input");
    for (var i = 0; i <= cll.length-1; i++) {
      cll[i].disabled=false;
    }
    lastOp=op;
  }else if (op=="sub"&&lastOp==op) {

    nMax=1;

    console.log("Subtraindo");
    var cll=document.querySelectorAll("div#storage input");
    for (var i = 0; i <= cll.length-1; i++) {
      cll[i].disabled=false;
    }
    lastOp=op;
  }else if (op=="swt"&&lastOp==op) {

    nMax=2;

    console.log("trocando");
    var cll=document.querySelectorAll("div#storage input");
    for (var i = 0; i <= cll.length-1; i++) {
      cll[i].disabled=false;
    }
    lastOp=op;
  }
  console.log(op+"__"+lastOp);
  

}

function pkgSelect(id){
  var cll = document.getElementById(id);

  if (selected<nMax || cll.checked==false) {
      if (cll.checked==true) {
        selected++;
        cll.checked=true;
      }else{
        cll.checked=false;
        selected--;
      }
  }else{
    cll.checked=false;
  }
  console.log(id + "  " + selected)
}
function Disable() {
  var cll=document.querySelectorAll("div#storage input");
    for (var i = 0; i <= cll.length-1; i++) {
      cll[i].disabled=true;
      cll[i].checked=false;
    }


}
function atualizeStt(id,stt){
var cll = document.getElementById(id);
  if(stt=="on"){
    cll.classList.remove('bg-danger');
    cll.classList.remove('border-danger');
    cll.classList.add('bg-success');
    cll.classList.add('border-success');
    cll.innerHTML="Conectado"
  }else{
    cll.classList.remove('bg-success');
    cll.classList.remove('border-success');
    cll.classList.add('bg-danger');
    cll.classList.add('border-danger');
    cll.innerHTML="Desconectado"
    }
}
function mqttLastMsg(msg,style){
    var cll = document.getElementById("msg_hist");
    var msg1 = document.getElementById("msg_1");
    var newElm = document.createElement("div");
    var msgCtt = document.createTextNode(msg);

    
    newElm.appendChild(msgCtt);
    
    newElm.classList.add('h3',"border","rounded-3","border-"+ style,"bg-"+ style,"text-light","p-1");
    
    msg1.after(newElm);
}

function XYvalue(){
    var cll=document.querySelectorAll("div#storage input");
    var div=document.getElementById("storage").querySelectorAll("input[type=checkbox]:checked");
    if(div.length >0){
      for (var i = 0; i <= cll.length-1; i++) {
     if( cll[i].checked==true){
        switch(i){
            case 0:
            return "*1x1";
            break;
            case 1:
            return "*1x2";
            break;
            case 2:
            return "*1x3";
            break;
            case 3:
            return "*2x1";
            break;
            case 4:
            return "*2x2";
            break;
            case 5:
            return "*2x3";
            break;
            case 6:
            return "*3x1";
            break;
            case 7:
            return "*3x2";
            break;
            case 8:
            return "*3x3";
            break;
            case 9:
            return "*4x1";
            break;
            case 10:
            return "*4x2";
            break;
            case 11:
            return "*4x3";
            break;
        }
     }
    }
    }else{
      return "0";
    }
}
var waitTime;
function checkOn(){
    waitTime = setTimeout(turnOff,10000);
    var lastStg = new Boolean(false);
    var cll = document.getElementById("op_stt").classList; 
    if (onStt==true) {
      for (var i = 0; i <= cll.length-1; i++) {
          if(cll[i] == "bg-success"){
              lastStg = true;
              console.log("true");
              break;
          }else if(cll[i] == "bg-danger"){
              lastStg = false;
              console.log("false");
          }
      }
      if(lastStg == false){
          atualizeStt("op_stt","on");
          mqttLastMsg("Heimdall conectado","success");
          console.log("Heimdall conectado");
      }
 
    }
    
}

function turnOff(){
    var lastStg = new Boolean(false);
    var cll = document.getElementById("op_stt").classList;
    
    for (var i = 0; i <= cll.length-1; i++) {
          if(cll[i] == "bg-danger"){
              lastStg = true;
              console.log("true-turnoff");
              break;
          }else if(cll[i] == "bg-success"){
              lastStg = false;
              console.log("false-turnoff");
          }
      }
    if(lastStg == false){
        atualizeStt("op_stt","off");
        mqttLastMsg("Heimdall desconectado","danger");
        console.log("Heimdall desconectado");
    }
}