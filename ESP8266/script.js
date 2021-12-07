const DESKTOP_WIDTH = 768;
const HOST_URL = window.location.origin;
const SPEED_MAX = 255;
const SPEED_MIN = 0;
const BRIGHT_MAX = 255;
const BRIGHT_MIN = 0;
const EYEOPEN = `<svg {{e}} viewBox="0 0 24 24" fill="none" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="eye"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"></path><circle cx="12" cy="12" r="3"></circle></svg>`
const EYECLOSE = `<svg {{e}} viewBox="0 0 24 24" fill="none" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="eye"><path d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"></path><line x1="1" y1="1" x2="23" y2="23"></line></svg>`;

var Socket;
var state = {
    headLight: 0,
    speed: 0,
    brightness: 0,
    navbar: 0,
    jConn: false
};

var controllerState = {
    keyboardS: false,
    joystickS: false
};

//General Functions
function d(i) {
    return document.getElementById(i);
}
function dae(o, e, f) {
    o.addEventListener(e, f);
}
function dar(o, e, f) {
    o.removeEventListener(e, f);
}
function dc(c, i) {
    return document.getElementsByClassName(c)[i || 0];
}
function dca(c) {
    return document.getElementsByClassName(c);
}
function dnone(o) {
    o.style.display = 'none';
}
function dflex(o) {
    o.style.display = 'flex';
}
const er = (m) => {
    console.log(m);
}
const disEn = (o, status) => {
    o.disabled = status;
}
const _send = (head, data) => {
    Socket.send(head + data);
}
function copy(aObject) {
    if (!aObject) {
        return aObject;
    }

    let v;
    let bObject = Array.isArray(aObject) ? [] : {};
    for (const k in aObject) {
        v = aObject[k];
        bObject[k] = (typeof v === "object") ? copy(v) : v;
    }

    return bObject;
}

const toNum = (st) => {
    return parseFloat(st.substring(0, st.lastIndexOf('px')))
}

const validateCred = (SSID, PASS) => {
    if (!SSID.match(/^[^!#;+\]\/"\t][^+\]"\t]{0,31}$/)) {
        return false;
    } if ((PASS == "") || !PASS.match(/^[ -~]{8,63}$/)) {
        return false;
    }
    return true;
}

function animate({ timing, draw, duration }) {
    let start = performance.now();
    requestAnimationFrame(function animate(time) {
        let timeFraction = (time - start) / duration;
        if (timeFraction > 1) timeFraction = 1;
        let progress = timing(timeFraction)
        draw(progress);
        if (timeFraction < 1) {
            requestAnimationFrame(animate);
        }
    });
}
// Custom

const connect = () => {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    var connection = d('connection');

    Socket.onmessage = (event) => {
        er(event.data);
    }
    Socket.onopen = () => {
        er("Socket connected.");
        connection.innerHTML = `Online <div class='dot dot-online'></div>`;
    }
    Socket.onclose = (e) => {
        er('Socket is closed. Reattempting..');
        connection.innerHTML = `Retry <div class='dot dot-reconn'></div>`;
        setTimeout(function () {
            connect();
        }, 250);
    };

    Socket.onerror = (err) => {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        connection.innerHTML = `Offline <div class='dot dot-offline'></div>`;
        Socket.close();
    };
};

const navBarSlide = () => {
    var navBar = dc('nav-bar');
    var dir = state.navbar == 1 ? 0 : 1;
    state.navbar = dir;
    animate({
        duration: 1000,
        timing: function (timeFraction) {
            return val = (1 / (1 + Math.exp(-(30 * timeFraction - 5))));
        },
        draw: function (progress) {
            var prog = Math.abs(dir - progress) * (-100);

            if (Math.abs(prog) > 96) {
                dnone(navBar);
                state.navbar = 0;
                updateAllHeights();
            } else if (Math.abs(prog) < 5) {
                dflex(navBar);
                state.navbar = 1;
                updateAllHeights();
            } else {
                dflex(navBar);
                state.navbar = dir;
            }
            navBar.style.transform = `translateX(${prog}%)`;
        }
    });
}

const closeNavBar = () => {
    if (state.navbar) { navBarSlide(); }
}

const showGamepad = () => {
    var statusPage = dc('settings');
    var gamepad = dc('gamepad');
    var setup = dc('setup');
    keyEvents(controllerState.keyboardS);
    joyEvents(controllerState.joystickS);
    dnone(statusPage);
    dnone(setup);
    dflex(gamepad);
}

const showSettings = () => {
    dnone(dc('gamepad'));
    dnone(dc('setup'));
    dflex(dc('settings'));
    keyEvents(false);
}

const showSetup = () => {
    keyEvents(false);

    dnone(dc('gamepad'));
    dnone(dc('settings'));
    dflex(dc('setup'));
}


dae(dc('nav-hamburger'), 'click', () => {
    navBarSlide();
    showGamepad();
});

dae(dc('gamepad'), 'click', () => {
    closeNavBar();
    showGamepad();
});



dae(d('setup'), 'click', () => {
    closeNavBar();
    showSetup();
});

dae(d('setting'), 'click', () => {
    closeNavBar();
    showSettings();
});

dae(d('SUBMIT_STA'), 'click', () => {
    var SSID = d('SSID_STA').value;
    var PASS = d('PASS_STA').value;
    if (validateCred(SSID, PASS)) {
        fetch(`${HOST_URL}/cred`, {
            method: "POST",
            headers: {
                'Content-type': 'application/x-www-form-urlencoded'
            },
            body: `type=STA&ssid=${SSID}&pass=${PASS}`
        }).then(response => response.text()).then((data) => {
            d('SSID_STA').value = "";
            d('PASS_STA').value = "";
        });
    } else {
        d('SSID_STA').value = "";
        d('PASS_STA').value = "";
        er("Enter Correct Creds");
    }
});

dae(d('SUBMIT_AP'), 'click', () => {
    var SSID = d('SSID_AP').value;
    var PASS = d('PASS_AP').value;
    if (validateCred(SSID, PASS)) {
        fetch(`${HOST_URL}/cred`, {
            method: "POST",
            headers: {
                'Content-type': 'application/x-www-form-urlencoded'
            },
            body: `type=AP&ssid=${SSID}&pass=${PASS}`
        }).then(response => response.text()).then((data) => {
            d('SSID_AP').value = "";
            d('PASS_AP').value = "";
        });
    } else {
        d('SSID_AP').value = "";
        d('PASS_AP').value = "";
        er("Enter Correct Creds");
    }
});

const showPass = (self, id) => {
    let di = d(id);
    di.type = (di.type == "password") ? "text" : "password";
    let eyeC = EYECLOSE.replace('{{e}}', `onclick="showPass(this,'${id}')"`);
    let eyeO = EYEOPEN.replace('{{e}}', `onclick="showPass(this,'${id}')"`);
    let eyeRep = (di.type == "text") ? eyeO : eyeC;
    self.parentElement.innerHTML = eyeRep;
}

const updateState = (data) => {
    state = {
        ...state,
        ...data
    };
    connect();
    headLight();
    brightness();
    speed();
}

const resetState = () => {
    fetch(`${HOST_URL}/resetstate`).then(res => res.text()).then(() => {
        fetch(`${HOST_URL}/state`).then(res => res.json()).then(data => updateState(data));
    });
}
dae(d('reset-state'), 'click', resetState);

const resetCreds = () => {
    fetch(`${HOST_URL}/resetcreds`).then(res => res.text());
}
dae(d('reset-creds'), 'click', resetCreds);

const resetAll = () => {
    resetState();
    resetCreds();
}
dae(d('reset-all'), 'click', resetAll);

const updateAllHeights = () => {
    const setHeight = (o, h) => { o.style.height = h };
    var headerCss = getComputedStyle(dc('header'));
    var containerCss = getComputedStyle(dc('container'));
    var contentHeight = (toNum(containerCss.height) - toNum(headerCss.height)) + 'px';
    setHeight(dc('nav-bar'), contentHeight);
    setHeight(dc('gamepad'), contentHeight);
    setHeight(dc('settings'), contentHeight);
    setHeight(dc('setup'), contentHeight);

}

dae(window, 'resize', updateAllHeights);

const hlToggle = () => {
    state.headLight = 1 - state.headLight;
    headLight();
    sendCommand('s', 'h', state.headLight);
}

const headLight = () => {
    var icon = d("headLight-icon");
    if (state.headLight == 1) {
        icon.style.display = 'inline';
    } else {
        icon.style.display = 'none';
    }
}

const brightness = () => {
    d('brightness').value = state.brightness;
    state.brightness = parseInt(d('brightness').value);
    var color = getComputedStyle(document.querySelector(':root')).getPropertyValue('--headLight-glow');
    var icon = d("headLight-icon");
    icon.style.filter = `drop-shadow(0 0 ${4 * state.brightness / 100}px ${color})`;
}

const changeBrightness = (val) => {
    sendCommand('s', 'b', val);
}

const speed = () => {
    dc('speed').value = state.speed;
    state.speed = parseInt(dc('speed').value);
    d('speed-val').innerHTML = state.speed;
}

const changeSpeed = (val) => {
    sendCommand('s', 's', val);
}
dae(d('brightness'), 'input', (e) => {
    state.brightness = e.target.value;
    brightness();
});
dae(dc('speed'), 'input', (e) => {
    state.speed = e.target.value;
    speed();
});


const getTheme = () => {
    return localStorage.getItem('theme');
}
const setTheme = (themeName) => {
    localStorage.setItem('theme', themeName);
    document.documentElement.className = themeName;
    d('darkmode').innerHTML = themeName == 'darkMode' ? 'Light Mode' : 'Dark Mode';
}
const toggleTheme = () => setTheme(getTheme() != 'darkMode' ? 'darkMode' : 'lightMode');

dae(d('darkmode'), 'click', toggleTheme);


const sendCommand = (uri, q1, q2) => {
    _send(uri[0] + q1, q2);
}

const forwardO = () => sendCommand('c', 0, 1);
const reverseO = () => sendCommand('c', 1, 1);
const leftO = () => sendCommand('c', 2, 1);
const rightO = () => sendCommand('c', 3, 1);

const forwardF = () => sendCommand('c', 0, 0);
const reverseF = () => sendCommand('c', 1, 0);
const leftF = () => sendCommand('c', 2, 0);
const rightF = () => sendCommand('c', 3, 0);

const stop = () => sendCommand('o', 's', 0);

btnEvents = [
    {
        t: 'forward',
        o: forwardO,
        f: forwardF
    },
    {
        t: 'right',
        o: rightO,
        f: rightF
    },
    {
        t: 'backward',
        o: reverseO,
        f: reverseF
    },
    {
        t: 'left',
        o: leftO,
        f: leftF
    }
];
const validDs = (self) =>{
    self.value = self.value.replaceAll(/[+-.]/g, '');
    if(self.placeholder == 'MM' || self.placeholder == 'SS'){
        if(parseInt(self.value) > 59){
            self.value = 59;
        }
    }
}
const getDSTime = () =>{
    let h = d('dsHH');
    let m = d('dsMM');
    let s = d('dsSS');
    let time = 0;
    time = (h.value === "")? 0 : parseInt(h.value)*3600;
    time += (m.value === "")? 0 : parseInt(m.value)*60;
    time += (s.value === "")? 0 : parseInt(s.value);
    time = (time === 0)? 1 : time;
    h.value = "";
    m.value = "";
    s.value = "";
    return time;
}
dae(d('timed-ds'), 'click', ()=>{
    sendCommand('s','d',getDSTime());
});
dae(document, 'DOMContentLoaded', () => {
    // nav height
    updateAllHeights();

    btnEvents.forEach((i) => {
        dae(dc(i.t), 'mousedown', i.o);
        dae(dc(i.t), 'mouseup', i.f);

        dae(dc(i.t), 'touchStart', i.o);
        dae(dc(i.t), 'touchend', i.f);
    });


    dae(dc('speed'), 'keydown', (e) => { e.preventDefault(); });
    dae(dc('stop'), 'click', () => { stop(); fetch(`${HOST_URL}/off`); });


    keyInit();
    joyInit();

    dae(window, "gamepadconnected", () => {
        er('gamepad connected');
        state.jConn = true;
        joyEvents(controllerState.joystickS);
        createGVector();
    });
    dae(window, "gamepaddisconnected", () => {
        state.jConn = false;
        joyEvents(controllerState.joystickS);
        clearProgress();
    });


    fetch(`${HOST_URL}/state`).then(
        (response) => {
            if (response.ok) { return response.json(); }
            else {
                throw new Error("Please reset status...");
            }
        }).then((data) => {
            updateState(data);
            //Event Listeners

            dae(d('headlight'), 'click', hlToggle);

            dae(d('brightness'), 'change', (e) => {
                changeBrightness(e.target.value);
            });

            dae(dc('speed'), 'change', (e) => {
                changeSpeed(e.target.value);
            });
            dae(d('restart'), 'click', () => sendCommand('s', 'r', 0));
            dae(d('deepSleep'), 'click', () => sendCommand('s', 'd', 0));

        }).catch(error => er(error));

});


//table local storage
//common fns
const saveTable = (param, table) => {
    localStorage.setItem(param, JSON.stringify(table));
}
const getTable = (param) => {
    return JSON.parse(localStorage.getItem(param));
}
const loadTable = (data, name) => {
    let cont = copy(data);
    let table = getTable(name);
    if (!table) {
        saveTable(name, data);
    } else {
        for (var i = 0; i < cont.length; i++) {
            cont[i].key = table[i].key;
        }
    }
    return cont;
}
const handleController = (e) => {
    let checked = e.target.checked;
    for (var item in controllerState) {
        controllerState[item] = false;
        try {
            d(item).checked = false;
        } catch (e) { }
    }
    d(e.target.id).checked = checked;
    controllerState[e.target.id] = checked;
    joyEvents(controllerState.joystickS);
}
//keyboard functions
const kConDefault = [{ type: 'forward', command: 'forward', key: 'ArrowUp', KEYDOWN: forwardO, KEYUP: forwardF }, { type: 'reverse', command: 'reverse', key: 'ArrowDown', KEYDOWN: reverseO, KEYUP: reverseF }, { type: 'right', command: 'right', key: 'ArrowRight', KEYDOWN: rightO, KEYUP: rightF }, { type: 'left', command: 'left', key: 'ArrowLeft', KEYDOWN: leftO, KEYUP: leftF }, { type: 'stop', command: 'Stop', key: 'Space', KEYDOWN: stop }, { type: 'light', command: 'Headlight', key: 'KeyB', KEYDOWN: () => { keyBrightness('press'); } }, { type: 'brightnessp', command: 'Headlight Brightness +', key: 'KeyH', KEYDOWN: () => { keyBrightness('plus'); } }, { type: 'brightnessm', command: 'Headlight Brightness -', key: 'KeyN', KEYDOWN: () => { keyBrightness('minus'); } }, { type: 'speedp', command: 'Speed +', key: 'NumpadAdd', KEYDOWN: () => { keySpeed('plus'); } }, { type: 'speedm', command: 'Speed -', key: 'NumpadSubtract', KEYDOWN: () => { keySpeed('minus'); } }]
var kCont;


const keySpeed = (type) => {
    switch (type) {
        case 'plus':
            if (state.speed < SPEED_MAX) {
                state.speed += 10;
                speed();
                changeSpeed(state.speed);
            }
            break;
        case 'minus':
            if (state.speed > SPEED_MIN) {
                state.speed -= 10;
                speed();
                changeSpeed(state.speed);
            }
            break;
    }
}

const keyBrightness = (type) => {
    switch (type) {
        case 'press':
            hlToggle();
            break;
        case 'plus':
            if (state.brightness < BRIGHT_MAX) {
                state.brightness += 10;
                brightness();
                changeBrightness(state.brightness);
            }
            break;
        case 'minus':
            if (state.brightness > BRIGHT_MIN) {
                state.brightness -= 10;
                brightness();
                changeBrightness(state.brightness);
            }
            break;
    }
}

const keyControl = (e) => {
    kCont.forEach((item) => {
        if (item.key == e.code) {
            item[e.type.toUpperCase()]();
        }
    });
}

const getkeyObj = (type) => {
    var ele;
    for (var i = 0; i < kCont.length; i++) {
        if (kCont[i].type == type) {
            ele = kCont[i];
        }
    }
    return ele;
}

const keyEvents = (flag) => {
    if (flag) {
        dae(document, 'keydown', keyControl);
        dae(document, 'keyup', keyControl);
    } else {
        dar(document, 'keydown', keyControl);
        dar(document, 'keyup', keyControl);
    }
}

const renderKeys = () => {
    const kc = d('keyboard-controls');
    let table = `<tr class="table-head"><th>Control</th><th>Key</th></tr>`;
    kCont.forEach((item) => {
        table += `<tr class="table-body" id="${item.type}p"><td>${item.command}</td><td id="${item.type}" onclick="editKeyTable(this)">${item.key}</td></tr>`;
    });
    kc.innerHTML = table;
}

const editKeyTable = (self) => {
    self.parentElement.innerHTML = `<td>${getkeyObj(self.id).command}</td><td id="${self.id}"><input type='text' class='table-text' onkeydown="setKey(event, this.parentElement.id)" placeholder="press a key"></td>`;
    d(self.id).childNodes[0].focus();
}
function validateKeyPress(k) {
    var valid = (k > 47 && k < 58) || // number keys
        k == 32 || k == 13 ||// spacebar & return key(s) (if you want to allow carriage returns)
        (k > 36 && k < 41) ||//arrow keys
        (k > 64 && k < 91) || // letter keys
        (k > 95 && k < 112) || // numpad keys
        (k > 185 && k < 193) || // ;=,-./` (in order)
        (k > 218 && k < 223); // [\]' (in order)
    return valid;
}
function keyNotExists(key) {
    let flag = true;
    kCont.forEach((item) => {
        if (item.key == key) { flag = false; }
    });
    return flag;
}

const setKey = (e, id) => {
    e = e || window.event;
    e.preventDefault();
    var charCode = (e.charCode) ? e.charCode : e.keyCode;

    if (validateKeyPress(charCode) && keyNotExists(e.code)) {
        for (var i = 0; i < kCont.length; i++) {
            if (kCont[i].type == id) {
                kCont[i].key = e.code;
            }
        }
        saveTable('keyControl', kCont);
        d(id + 'p').innerHTML = `<td>${id}</td><td id='${id}' onclick='editKeyTable(this)'>${e.code}</td>`;
    } else {
        renderKeys();
    }
}
const resetKeys = () => {
    saveTable('keyControl', kConDefault);
    kCont = copy(kConDefault);
    renderKeys();
}
dae(d('key-reset'), 'click', resetKeys);


const keyInit = () => {
    kCont = loadTable(kConDefault, 'keyControl');
    renderKeys();
}
// joystick controls

const joyDefault = [{
    type: 'forward',
    command: 'Forward/Reverse',
    key: 'a1',
    sent: {
        forwardO: false,
        reverseO: false
    }
},
{
    type: 'right',
    command: 'Left/Right',
    key: 'a0',
    sent: {
        leftO: false,
        rightO: false
    }
},
{
    type: 'stop',
    command: 'Stop',
    key: 'b2',
    KEYDOWN: stop
},
{
    type: 'light',
    command: 'Headlight',
    key: 'b0',
    KEYDOWN: () => { keyBrightness('press'); }
},
{
    type: 'brightnessp',
    command: 'Headlight Brightness +',
    key: 'b5',
    KEYDOWN: () => { keyBrightness('plus'); }
},
{
    type: 'brightnessm',
    command: 'Headlight Brightness -',
    key: 'b7',
    KEYDOWN: () => { keyBrightness('minus'); }
},
{
    type: 'speedp',
    command: 'Speed +',
    key: 'b4',
    KEYDOWN: () => { keySpeed('plus'); }
},
{
    type: 'speedm',
    command: 'Speed -',
    key: 'b6',
    KEYDOWN: () => { keySpeed('minus'); }
}]
var joyCon;

var gVector = [];

const getGObj = (index) => {
    var ele;

    for (var i = 0; i < gVector.length; i++) {

        if (gVector[i].index == index) {
            ele = gVector[i];
        }
    }

    return ele;
}

const joyEvents = (flag) => {
    if (flag) {
        if (state.jConn) {
            update();
        }
    } else {
        cancelAnimationFrame(joyUpdate);
        clearProgress();
    }
}

const renderJoy = () => {
    const jc = d('joystick-controls');
    let table = `<tr class="table-head"><th>Control</th><th>Channel</th></tr>`;
    joyCon.forEach((item) => {
        table += `<tr class="table-body" id="${item.type}j"><td>${item.command}</td><td id="${item.type}"><div class="progress" id="p${item.key}"><div><div></div></div></td></tr>`;
    });
    jc.innerHTML = table;
}

const clearProgress = () => {
    joyCon.forEach((item) => {
        d(`p${item.key}`).childNodes[0].childNodes[0].style.width = `0%`;
    });
}


const clearSent = (item) => {
    for (var i in item) {
        item[i] = false;
    }
}
const joyLogic = () => {
    joyCon.forEach((item) => {
        var obj = getGObj(item.key);
        if (dc('settings').style.display == 'flex') {
            d(`p${item.key}`).childNodes[0].childNodes[0].style.width = `${(obj.curr) * 100}%`;
        }
        try {
            if (obj.type == 'axis') {
                switch (item.command) {
                    case 'Forward/Reverse':
                        if (obj.curr > 0.7) {
                            if (!item.sent.reverseO) {

                                clearSent(item.sent);
                                item.sent.reverseO = true;
                                reverseO();
                            }
                        } else if (obj.curr < 0.3) {
                            if (!item.sent.forwardO) {

                                clearSent(item.sent);
                                item.sent.forwardO = true;

                                forwardO();
                            }
                        } else {
                            if (item.sent.forwardO) {
                                clearSent(item.sent);

                                forwardF();

                            } else if (item.sent.reverseO) {
                                clearSent(item.sent);

                                reverseF();
                            }
                        }
                        break;
                    case 'Left/Right':
                        if (obj.curr > 0.7) {
                            if (!item.sent.rightO) {
                                clearSent(item.sent);
                                item.sent.rightO = true;
                                rightO();
                            }
                        } else if (obj.curr < 0.3) {
                            if (!item.sent.leftO) {
                                clearSent(item.sent);
                                item.sent.leftO = true;
                                leftO();
                            }
                        } else {
                            if (item.sent.leftO) {
                                clearSent(item.sent);
                                leftF();
                            } else if (item.sent.rightO) {
                                clearSent(item.sent);
                                rightF();
                            }
                        }
                        break;
                }
            } else {
                if (obj.curr - obj.prev == 1) {
                    item.KEYDOWN();
                }
            }
        } catch (e) { }
    });
}
var joyUpdate;
const update = () => {
    for (const gamepad of navigator.getGamepads()) {
        var i = 0;
        if (gamepad) {
            for (const [index, axis] of gamepad.axes.entries()) {
                gVector[i].prev = gVector[i].curr;
                gVector[i].curr = axis * 0.5 + 0.5;
                i++;
            }
            for (const [index, button] of gamepad.buttons.entries()) {
                gVector[i].prev = gVector[i].curr;
                gVector[i].curr = button.value;
                i++;
            }
            joyLogic();
            break;
        }
    }
    joyUpdate = requestAnimationFrame(update);
}
const createGVector = () => {
    gVector = [];
    for (const gamepad of navigator.getGamepads()) {
        if (gamepad) {
            for (const [index, axis] of gamepad.axes.entries()) {
                var gAxis = {
                    type: 'axis',
                    index: `a${index}`,
                    prev: axis,
                    curr: axis
                };
                gVector.push(gAxis);
            }
            for (const [index, button] of gamepad.buttons.entries()) {
                var gButton = {
                    type: 'button',
                    index: `b${index}`,
                    prev: button.value,
                    curr: button.value
                }
                gVector.push(gButton);
            }
            break; //1 controller
        }
    }
}

const joyInit = () => {
    joyCon = loadTable(joyDefault, 'joyControl');
    renderJoy();
}




//IIFE

(function () {
    toggleTheme();
    toggleTheme();
})();

(function () {

    var arrow = `<svg viewBox="0 0 24 24" fill="none" stroke-linecap="round"
                stroke-linejoin="round">
                <line x1="12" y1="19" x2="12" y2="5"></line>
                <polyline points="5 12 12 5 19 12"></polyline>
                </svg>`;
    var j = 0;
    btnEvents.forEach((i) => {
        dc(i.t).innerHTML = arrow;
        dc(i.t).childNodes[0].style = `transform:rotate(${90 * (j++)}deg)`;
    });

    let pass = dca('pass');
    for (var i = 0; i < pass.length; i++) {
        let id = pass[i].id.substring(1);
        pass[i].innerHTML += EYECLOSE.replace("{{e}}", `onclick="showPass(this,'${id}')"`);
    }

})();