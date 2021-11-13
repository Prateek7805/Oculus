const DESKTOP_WIDTH = 768;
const commType = 'Socket'
const HOST_URL = window.location.origin;
var Socket;
var state = {
    headLight: 1,
    speed: 100,
    brightness: 100,
    commType: 0,
    darkMode: 1,
    navbar: 0
};

//General Functions
function d(i) {
    return document.getElementById(i);
}
function dae(o, e, f) {
    o.addEventListener(e, f);
}
function dc(c, i) {
    return document.getElementsByClassName(c)[i || 0];
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

const _send = (head, data) => {
    Socket.send(head + data);
}

const toNum = (st) => {
    return parseFloat(st.substring(0, st.lastIndexOf('px')))
}

const validateCred = (SSID, PASS) => {
    if (!SSID.match(/^[A-z0-9_]{1,32}$/)) {
        return false;
    } if ((PASS == "") || !PASS.match(/^[^\n ]{8,63}$/)) {
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
            } else if (Math.abs(prog) < 5) {
                dflex(navBar);
                state.navbar = 1;
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

const showStatus = () => {
    var statusPage = dc('status-page');
    var gamepad = dc('gamepad');
    var setup = dc('setup');
    fetch(`${HOST_URL}/state`).then(

        (response) => {
            if (response.ok) { return response.json() }
            else {
                throw new Error("please reset state");
            }
        }).then((data) => {
            var statusData = ""
            for (var key in data) {
                statusData += `<p>${key} : ${data[key]}</p>`;
            }
            statusPage.innerHTML = statusData;
            dflex(statusPage);
            dnone(gamepad);
            dnone(setup);
        }).catch(error => er(error));
}
const showGamepad = () => {
    var statusPage = dc('status-page');
    var gamepad = dc('gamepad');
    var setup = dc('setup');
    var resetPage = dc('reset-page');
    dnone(statusPage);
    dnone(setup);
    dnone(resetPage);
    dflex(gamepad);
}


const showSetup = () => {
    dnone(dc('gamepad'));
    dnone(dc('status-page'));
    dflex(dc('setup'));
}

const showReset = () => {
    dnone(dc('gamepad'));
    dnone(dc('status-page'));
    dflex(dc('reset-page'));
}

dae(dc('nav-hamburger'), 'click', () => {
    navBarSlide();
    showGamepad();
});

dae(dc('gamepad'), 'click', () => {
    closeNavBar();
    showGamepad();
});
dae(dc('status-page'), 'click', () => {
    closeNavBar();
    showGamepad();
});

dae(d('setup'), 'click', () => {
    closeNavBar();
    showSetup();
});
dae(d('reset-data'), 'click', () => {
    closeNavBar();
    showReset();
});


dae(d('SUBMIT_STA'), 'click', () => {
    var SSID = d('SSID_STA').value;
    var PASS = d('PASS_STA').value;
    if (validateCred(SSID, PASS)) {
        fetch(`${HOST_URL}/cred?q1=STA&q2=${SSID}&q3=${PASS}`).then(response => response.text()).then((data) => {
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
        fetch(`${HOST_URL}/cred?q1=AP&q2=${SSID}&q3=${PASS}`).then(response => response.text()).then((data) => {
            d('SSID_AP').value = "";
            d('PASS_AP').value = "";
        });
    } else {
        d('SSID_AP').value = "";
        d('PASS_AP').value = "";
        er("Enter Correct Creds");
    }
});

const updateState = (data) => {
    state = {
        ...state,
        ...data
    };
    if (state.commType == 1) {
        connect();
    } else {
        d('connection').innerHTML = `Online <div class='dot dot-online'></div>`
    }
    headLight();
    brightness();
    speed();
}

const resetState = () =>{
    fetch(`${HOST_URL}/resetstate`).then(res => res.text()).then(()=>{
        fetch(`${HOST_URL}/state`).then(res => res.json()).then(data => updateState(data));
    });
}
dae(d('reset-state'), 'click', resetState);

const resetCreds=()=>{
    fetch(`${HOST_URL}/resetcreds`).then(res => res.text())
}
dae(d('reset-creds'), 'click', resetCreds);

const resetAll =()=>{
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
    setHeight(dc('status-page'), contentHeight);
    setHeight(dc('setup'), contentHeight);
}

dae(window, 'resize', updateAllHeights);

const headLight = () => {
    var icon = d("headLight-icon");
    er(state.headLight);
    if (state.headLight == 1) {
        icon.style.display = 'inline';
    } else {
        icon.style.display = 'none';
    }
}

const brightness = () => {
    d('brightness').value = state.brightness;
    var color = getComputedStyle(document.querySelector(':root')).getPropertyValue('--headLight-glow');
    var icon = d("headLight-icon");
    icon.style.filter = `drop-shadow(0 0 ${4 * state.brightness / 100}px ${color})`;
}

const speed = () => {
    dc('speed').value = state.speed;
    d('speed-val').innerHTML = state.speed;
}



dae(document, 'DOMContentLoaded', () => {
    // nav height
    updateAllHeights();

    

    dae(dc('forward'), 'mousedown', forwardO);

    dae(dc('backward'), 'mousedown', reverseO);

    dae(dc('left'), 'mousedown', leftO);

    dae(dc('right'), 'mousedown', rightO);

    dae(dc('forward'), 'mouseup', forwardF);

    dae(dc('backward'), 'mouseup', reverseF);

    dae(dc('left'), 'mouseup', leftF);

    dae(dc('right'), 'mouseup', rightF);

    dae(dc('forward'), 'touchstart', forwardO);

    dae(dc('backward'), 'touchstart', reverseO);

    dae(dc('left'), 'touchstart', leftO);

    dae(dc('right'), 'touchstart', rightO);

    dae(dc('forward'), 'touchend', forwardF);

    dae(dc('backward'), 'touchend', reverseF);

    dae(dc('left'), 'touchend', leftF);

    dae(dc('right'), 'touchend', rightF);

    dae(dc('stop'), 'click', stop);

    dae(d('status-data'), 'click', showStatus);

    dae(dc('speed'), 'input', (e) => {
        d('speed-val').innerHTML = e.target.value;
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

            dae(d('headlight'), 'click', () => { state.headLight = 1 - state.headLight; headLight(); sendCommand('st', 'h', state.headLight) });

            dae(d('brightness'), 'change', (e) => {
                state.brightness = e.target.value;
                sendCommand('st', 'b', e.target.value);
            });

            dae(dc('speed'), 'change', (e) => {
                sendCommand('st', 's', e.target.value);
            });
            dae(d('restart'), 'click', () => sendCommand('st', 'r', 0));
            dae(d('deepSleep'), 'click', () => sendCommand('st', 'd', 0));

        }).catch(error => er(error));

});

const sendCommand = (uri, q1, q2) => {
    if (state.commType == 1) {
        _send(uri[0] + q1, q2);
    } else {
        fetch(`${HOST_URL}/${uri}?q1=${q1}&q2=${q2}`).then(response => response.text()).then((data) => {
            er(data);
        });
    }
}

const forwardO = () => sendCommand('c', 0, 1);
const reverseO = () => sendCommand('c', 1, 1);
const leftO = () => sendCommand('c', 2, 1);
const rightO = () => sendCommand('c', 3, 1);

const forwardF = () => sendCommand('c', 0, 0);
const reverseF = () => sendCommand('c', 1, 0);
const leftF = () => sendCommand('c', 2, 0);
const rightF = () => sendCommand('c', 3, 0);

const stop = () => sendCommand('off', 's', 0);