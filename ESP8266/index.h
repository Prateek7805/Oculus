const char _index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="styles.css">
    <title>Document</title>
</head>

<body>
    <div class="container">
        <div class="header">
            <div class="row">
                <div class="nav-icon">
                    <button class="nav-hamburger c-pointer"><svg class="hamburger-icon" viewBox="0 0 24 24" fill="none"
                            stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                            <line x1="3" y1="12" x2="21" y2="12"></line>
                            <line x1="3" y1="6" x2="21" y2="6"></line>
                            <line x1="3" y1="18" x2="21" y2="18"></line>
                        </svg></button>
                </div>
                <div class="c3">
                    <p class="title">Oculus</p>
                </div>
                <div id="status" class="c3"></div>
            </div>
        </div>
        <div class="content">
            <div class="nav-bar">

                <div class="nav-tiles df-c">
                    <p class="t-center py px push-down" id="headlight">Headlight <svg id="headLight-icon"
                            viewBox="0 0 24 24" fill="none" stroke-linecap="round" stroke-linejoin="round">
                            <circle cx="12" cy="12" r="5"></circle>
                            <line x1="12" y1="1" x2="12" y2="3"></line>
                            <line x1="12" y1="21" x2="12" y2="23"></line>
                            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
                            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
                            <line x1="1" y1="12" x2="3" y2="12"></line>
                            <line x1="21" y1="12" x2="23" y2="12"></line>
                            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
                            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
                        </svg></p>
                    <input class="slider my w-100" type="range" id="brightness" max=1023 min=0 />
                </div>
                <div class="nav-tiles" id="connection">
                    Offline <div class="dot dot-offline"></div>
                </div>
                <div class="nav-tiles push-down" id="restart">
                    Restart
                </div>
                <div class="nav-tiles push-down" id="deepSleep">
                    Deep sleep
                </div>
                <div class="nav-tiles push-down" id="status-data">
                    status
                </div>
                <div class="nav-tiles push-down" id="setup">
                    Credentials
                </div>
                <div class="nav-tiles push-down" id="reset-data">
                    Reset Data
                </div>
            </div>

            <div class="gamepad">
                <div class="row-center">
                    <button class="forward g-btn"><svg viewBox="0 0 24 24" fill="none" stroke-linecap="round"
                            stroke-linejoin="round">
                            <line x1="12" y1="19" x2="12" y2="5"></line>
                            <polyline points="5 12 12 5 19 12"></polyline>
                        </svg></button>
                </div>
                <div class="row-center">
                    <button class="left g-btn"><svg viewBox="0 0 24 24" fill="none" stroke-linecap="round"
                            stroke-linejoin="round">
                            <line x1="19" y1="12" x2="5" y2="12"></line>
                            <polyline points="12 19 5 12 12 5"></polyline>
                        </svg></button><button class="stop g-btn"><svg viewBox="0 0 24 24" fill="none"
                            stroke-linecap="round" stroke-linejoin="round">
                            <circle cx="12" cy="12" r="10"></circle>
                        </svg></button><button class="right g-btn"><svg viewBox="0 0 24 24" fill="none"
                            stroke-linecap="round" stroke-linejoin="round">
                            <line x1="5" y1="12" x2="19" y2="12"></line>
                            <polyline points="12 5 19 12 12 19"></polyline>
                        </svg></button>
                </div>
                <div class="row-center">
                    <button class="backward g-btn"><svg viewBox="0 0 24 24" fill="none" oke-linecap="round"
                            stroke-linejoin="round">
                            <line x1="12" y1="5" x2="12" y2="19"></line>
                            <polyline points="19 12 12 19 5 12"></polyline>
                        </svg></button>
                </div>

                <div class="row-center my">
                    <input type="range" class="slider speed" max="1023" min="0" value="0">
                    <p id="speed-val" class="text">0</p>
                </div>
            </div>
            <div class="status-page d-n">

            </div>
            <div class="setup">
                <div>
                    <div class="card">
                        <p class="text t-center">STA Credentials</p>
                        <div class="dfe">
                            <input type="text" class="card-textbox" placeholder="SSID" id="SSID_STA">
                        </div>
                        <div class="dfe">
                            <input type="password" class="card-textbox" placeholder="password" id="PASS_STA">
                        </div>
                        <div class="dfe">
                            <button class="card-btn" id="SUBMIT_STA">Submit</button>
                        </div>
                    </div>
                </div>

                <div>
                    <div class="card">
                        <p class="text t-center">AP Credentials</p>
                        <div class="dfe">
                            <input type="text" class="card-textbox" placeholder="SSID" id="SSID_AP">
                        </div>
                        <div class="dfe">
                            <input type="password" class="card-textbox" placeholder="password" id="PASS_AP">
                        </div>
                        <div class="dfe">
                            <button class="card-btn" id="SUBMIT_AP">Submit</button>
                        </div>
                    </div>
                </div>
            </div>

            <div class="reset-page">
                <div>
                    <div class="card">
                        <div class="dfe">
                            <p class="text">Reset State</p>
                        </div>
                        <div class="dfe">
                            <button class="card-btn" id="reset-state">Reset</button>
                        </div>
                    </div>
                </div>
                <div>
                    <div class="card">
                        <div class="dfe">
                            <p class="text">Reset Credentials</p>
                        </div>
                        <div class="dfe">
                            <button class="card-btn" id="reset-creds">Reset</button>
                        </div>
                    </div>
                </div>
                <div>
                    <div class="card">
                        <div class="dfe">
                            <p class="text">Factory Reset</p>
                        </div>
                        <div class="dfe">
                            <button class="card-btn" id="reset-all">Reset</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script src="script.js"></script>
</body>

</html>
)=====";