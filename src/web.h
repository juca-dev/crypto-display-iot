const char WEB_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Crypto Controller</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            border: none;
            box-sizing: border-box;
        }

        *,
        input,
        button {
            outline: none;
            font-family: "Poppins", sans-serif;
            box-sizing: border-box;
        }

        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: space-evenly;
            min-height: 100vh;
            background: #000;
        }

        main {
            position: relative;
            padding: 2.5em;
            background: #f3f3f3;
            border-radius: 0.5em;
            box-shadow: 0 0.5em 1.5em rgba(0, 0, 0, 0.5);
        }

        h1 {
            color: #000;
            text-shadow: 0 0 0.25em rgba(255, 255, 255, 0.75);
            font-size: xx-large;
        }

        fieldset {
            margin-bottom: 1em;
            padding: 1em;
            background: #fff;
            border-radius: 0.5em;
        }

        fieldset>div {
            position: relative;
            width: 50vw;
            height: 3em;
            margin-bottom: 1.5em;
        }

        fieldset>div:last-child {
            margin-bottom: 0;
        }

        fieldset>div>input
        , fieldset>div>textarea {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            border: 1px solid #000;
            background: transparent;
            padding: 1em;
            border-radius: 0.5em;
            font-size: medium;
            color: #000;
            font-weight: 300;
        }
        fieldset>div>textarea {
            height: 100%;
        }

        fieldset>div>label {
            position: absolute;
            top: 1px;
            left: 1px;
            padding: 1em;
            display: inline-block;
            font-size: medium;
            color: #000;
            font-weight: 300;
            transition: 0.5s;
            pointer-events: none;
        }

        fieldset>div>input:focus~label
        , fieldset>div>input:valid~label
        , fieldset>div>textarea:focus~label
        , fieldset>div>textarea:valid~label {
            transform: translateX(-1em) translateY(-2.25em);
            font-size: small;
        }

        fieldset>div>input[type="submit"] {
            background: #2196f3;
            color: #fff;
            max-width: 50%;
            cursor: pointer;
            font-weight: 500;
            border: none;
        }

        fieldset>div>input[type="submit"]:hover {
            background: #e91e63;
        }

        fieldset>div>input[type="color"] {
            height: 3em;
            padding: 0;
        }

        fieldset>div>input[type="range"] {
            padding: 1em 0;
        }

        legend {
            height: 1.5em;
            font-weight: 500;
            font-size: larger;
            padding-left: 0.5em;
            border-left: 0.25em solid #e91e63;
            margin-bottom: 1.5em;
        }

        .hide {
            display: none;
        }

        #loader {
            position: relative;
            width: 200px;
            height: 200px;
        }

        #loader::before {
            content: "";
            position: absolute;
            top: 85px;
            left: 15%;
            width: 70%;
            height: 70%;
            background: #ff3b8d;
            box-shadow: -60px -55px 0 #07b2ff;
            border-radius: 50%;
            animation: animate 2.5s linear infinite;
        }

        #loader::after {
            content: "";
            position: absolute;
            bottom: 85px;
            left: 15%;
            width: 70%;
            height: 70%;
            background: #8dff08;
            box-shadow: 60px 55px 0 #ffeb3b;
            border-radius: 50%;
            animation: animate 2.5s linear infinite;
        }

        #loader>span {
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: #fff;
            border-radius: 50%;
            z-index: 1;
            display: flex;
            align-items: center;
            justify-content: center;
            text-transform: uppercase;
            letter-spacing: 0.5em;
            font-weight: 500;
            font-size: large;
            text-align: center;
            color: rgba(0, 0, 0, 0.1);
        }

        #loader>span::before {
            content: "";
            position: absolute;
            top: 8px;
            left: 8px;
            right: 8px;
            bottom: 8px;
            background: transparent;
            border-radius: 50%;
            box-shadow: inset 0 0 20px rgba(0, 0, 0, 0.25);
        }

        @keyframes animate {
            0% {
                filter: blur(50px) hue-rotate(0deg);
            }

            100% {
                filter: blur(50px) hue-rotate(360deg);
            }
        }

        #loader~main {
            display: none;
        }
    </style>
</head>

<body>
    <header>
        <h1>Crypto Controller</h1>
    </header>
    <div id="loader">
        <span id="message">Loading</span>
    </div>
    <main>
        <form onsubmit="return setDisplay(event)">
            <fieldset>
                <legend>Display</legend>
                <div>
                    <input type="range" required min="0" max="128" value="0" id="display_x" />
                    <label for="display_x">X</label>
                </div>
                <div>
                    <input type="range" required min="0" max="32" value="0" id="display_y" />
                    <label for="display_y">Y</label>
                </div>
                <div>
                    <input type="range" required min="1" max="5" value="1" id="display_s" />
                    <label for="display_s">Size</label>
                </div>
                <div>
                    <input type="checkbox" value="1" id="display_w" />
                    <label for="display_w">
                        Wrap
                    </label>
                </div>
                <div>
                    <input type="submit" value="Save" />
                </div>
            </fieldset>
        </form>
        <form onsubmit="return setWifi(event)">
            <fieldset>
                <legend>WIFI</legend>
                <div>
                    <input type="text" required id="wifi_ssid" placeholder="SSID" />
                    <label for="wifi_ssid">SSID</label>
                </div>
                <div>
                    <input type="text" required id="wifi_password" placeholder="PASSWORD" />
                    <label for="wifi_password">PASSWORD</label>
                </div>
                <div>
                    <input type="submit" value="Connect" />
                </div>
            </fieldset>
        </form>
        <form onsubmit="return setIot(event)">
            <fieldset>
                <legend>Iot</legend>
                <div>
                    <input type="text" required id="iot_id" placeholder="ID" />
                    <label for="iot_id">ID</label>
                </div>
                <div>
                    <input type="text" required id="iot_host" placeholder="HOST" />
                    <label for="iot_host">HOST</label>
                </div>
                <div>
                    <input type="number" min="0" max="9999" id="iot_port" />
                    <label for="iot_port">PORT</label>
                </div>
                <div>
                    <input type="text" required id="iot_pub" placeholder="PUB" />
                    <label for="iot_pub">PUB</label>
                </div>
                <div>
                    <input type="text" required id="iot_sub" placeholder="SUB" />
                    <label for="iot_sub">SUB</label>
                </div>
                <div>
                    <textarea required id="iot_certCA" placeholder="CERT CA"></textarea>
                    <label for="iot_certCA">CERT CA</label>
                </div>
                <div>
                    <textarea required id="iot_certClient" placeholder="CERT Client"></textarea>
                    <label for="iot_certClient">CERT Client</label>
                </div>
                <div>
                    <textarea required id="iot_certKey" placeholder="CERT Key"></textarea>
                    <label for="iot_certKey">CERT Key</label>
                </div>
                <div>
                    <input type="submit" value="Save" />
                </div>
            </fieldset>
        </form>
    </main>
</body>
<script>
    const message = document.getElementById("message")
        , wifi_ssid = document.getElementById("wifi_ssid")
        , wifi_password = document.getElementById("wifi_password")
        , display_x = document.getElementById("display_x")
        , display_y = document.getElementById("display_y")
        , display_s = document.getElementById("display_s")
        , display_w = document.getElementById("display_w")
        , iot_id = document.getElementById("iot_id")
        , iot_host = document.getElementById("iot_host")
        , iot_port = document.getElementById("iot_port")
        , iot_certCA = document.getElementById("iot_certCA")
        , iot_certClient = document.getElementById("iot_certClient")
        , iot_certKey = document.getElementById("iot_certKey")
        , iot_pub = document.getElementById("iot_pub")
        , iot_sub = document.getElementById("iot_sub")
        ;

    async function delay(t) {
        return await new Promise((resolve) => {
            setTimeout(resolve, t);
        });
    }
    async function request(method, path, data) {
        const xhr = new XMLHttpRequest();
        xhr.timeout = 5000;
        const res = await new Promise((resolve, reject) => {
            xhr.onreadystatechange = () => {
                if (xhr.readyState == 4) {
                    if (xhr.status === 404) {
                        return resolve(null);
                    }
                    if (xhr.status < 200 || xhr.status >= 300) {
                        console.error(xhr);
                        return reject(xhr.responseText || `Status: ${xhr.status}`);
                    }
                    return resolve(xhr.responseText.length ? JSON.parse(xhr.responseText) : null);
                }
            };

            xhr.open(method, path, true);
            xhr.send(JSON.stringify(data));
        });

        await delay(500);
        return res;
    }
    function setWifi(ev) {
        if (ev) {
            ev.preventDefault();
        }

        const data = {
            ssid: wifi_ssid.value
            , password: wifi_password.value
        };

        request("POST", "/wifi", data);
        console.log("wifi updated: ", data);
        return false;
    }
    function setDisplay(ev) {
        if (ev) {
            ev.preventDefault();
        }

        const data = {
            x: +display_x.value
            , y: +display_y.value
            , s: +display_s.value
            , w: display_w.checked
        };

        request("POST", "/display", data);
        console.log("display updated: ", data);
        return false;
    }
    function setIot(ev) {
        if (ev) {
            ev.preventDefault();
        }

        const data = {
            id: iot_id.value
            , host: iot_host.value
            , port: +iot_port.value
            , certCA: iot_certCA.value
            , certClient: iot_certClient.value
            , certKey: iot_certKey.value
            , pub: iot_pub.value
            , sub: iot_sub.value
        };

        request("POST", "/iot", data);
        console.log("iot updated: ", data);
        return false;
    }
    async function getDisplay() {
        const res = await request("GET", "/display");
        console.log("display: ", res);
        if (!res) return;

        display_x.value = res.x;
        display_y.value = res.y;
        display_s.value = res.s;
        display_w.checked = res.w;
    }
    async function getWifi() {
        const res = await request("GET", "/wifi");
        console.log("wifi: ", res);
        if (!res) return;

        wifi_ssid.value = res.ssid;
        wifi_password.value = res.password;
    }
    async function getIot() {
        const res = await request("GET", "/iot");
        console.log("iot: ", res);
        if (!res) return;

        iot_id.value = res.id;
        iot_host.value = res.host;
        iot_port.value = res.port;
        iot_certCA.value = res.certCA;
        iot_certClient.value = res.certClient;
        iot_certKey.value = res.certKey;
        iot_pub.value = res.pub;
        iot_sub.value = res.sub;
    }
    async function load() {
        await getDisplay();
        await getIot();
        try {
            await getWifi();
        }
        catch {
            //problem when is connected
        }
    }
    window.onload = async () => {
        await delay(2500);
        let loading = 1;
        while (loading) {
            try {
                await load();
                loading = 0;
            } catch (err) {
                console.error(err);
                message.innerText = `Retry connect (${loading++})`;
                if (loading > 5) {
                    message.innerText = 'Can`t connect';
                    return;
                }
                await delay(2500);
            }
        }
        document.getElementById("loader").remove();
    };
</script>

</html>
)=====";
