const char WEB_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Led Controller</title>
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
      fieldset > div {
        position: relative;
        width: 50vw;
        height: 3em;
        margin-bottom: 1.5em;
      }
      fieldset > div:last-child {
        margin-bottom: 0;
      }
      fieldset > div > input {
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
      fieldset > div > label {
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
      fieldset > div > input:focus ~ label,
      fieldset > div > input:valid ~ label {
        transform: translateX(-1em) translateY(-2.25em);
        font-size: small;
      }
      fieldset > div > input[type="submit"] {
        background: #2196f3;
        color: #fff;
        max-width: 50%;
        cursor: pointer;
        font-weight: 500;
        border: none;
      }
      fieldset > div > input[type="submit"]:hover {
        background: #e91e63;
      }
      fieldset > div > input[type="color"] {
        height: 3em;
        padding: 0;
      }
      fieldset > div > input[type="range"] {
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
      #loader > span {
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
        color: rgba(0, 0, 0, 0.1);
      }
      #loader > span::before {
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
      #loader ~ main {
        display: none;
      }
    </style>
  </head>
  <body>
    <header>
      <h1>Led Controller</h1>
    </header>
    <div id="loader">
      <span>Loading</span>
    </div>
    <main>
      <form>
        <fieldset>
          <legend>Color</legend>
          <div>
            <input type="color" id="color_rgb" oninput="setRGB()" />
            <label for="color_rgb">RGB</label>
          </div>
          <div>
            <input
              type="range"
              min="0"
              max="254"
              value="0"
              id="color_w"
              oninput="setRGB()"
            />
            <label for="color_w">Intensity</label>
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
            <input
              type="text"
              required
              id="wifi_password"
              placeholder="PASSWORD"
            />
            <label for="wifi_password">PASSWORD</label>
          </div>
          <div>
            <input type="submit" value="Connect" />
          </div>
        </fieldset>
      </form>
      <form onsubmit="return setDevice(event)">
        <fieldset>
          <legend>Device</legend>
          <div>
            <input type="number" min="0" max="255" id="device_pin" />
            <label for="device_pin">PIN</label>
          </div>
          <div>
            <input type="number" min="0" max="9999" id="device_leds" />
            <label for="device_leds">Leds</label>
          </div>
          <div>
            <input type="submit" value="Save" />
          </div>
        </fieldset>
      </form>
    </main>
  </body>
  <script>
    const wifi_ssid = document.getElementById("wifi_ssid"),
      wifi_password = document.getElementById("wifi_password"),
      color_rgb = document.getElementById("color_rgb"),
      color_w = document.getElementById("color_w"),
      device_pin = document.getElementById("device_pin"),
      device_leds = document.getElementById("device_leds");

    function hexToRGB(hex) {
      const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
      return result
        ? {
            r: parseInt(result[1], 16),
            g: parseInt(result[2], 16),
            b: parseInt(result[3], 16),
          }
        : null;
    }
    function rgbToHex(data) {
      return (
        "#" +
        ((1 << 24) + (data.r << 16) + (data.g << 8) + data.b)
          .toString(16)
          .slice(1)
      );
    }
    async function delay(t) {
      return await new Promise((resolve) => {
        setTimeout(resolve, t);
      });
    }
    async function request(method, path, data) {
      const xhr = new XMLHttpRequest();
      const res = await new Promise((resolve, reject) => {
        xhr.onreadystatechange = () => {
          if (xhr.readyState == 4) {
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

      const ssid = wifi_ssid.value;
      const password = wifi_password.value;
      const data = { ssid: ssid, password: password };

      request("POST", "/wifi", data);
      console.log("wifi updated: ", data);
      return false;
    }
    function setRGB(ev) {
      if (ev) {
        ev.preventDefault();
      }

      const data = hexToRGB(color_rgb.value);
      data.a = +color_w.value;

      const value = `rgb(${data.r},${data.g},${data.b},${data.a / 255})`;
      document.body.style.backgroundColor = `${value}`;

      request("POST", "/rgb", data);
      console.log("rgb updated: ", data);
      return false;
    }
    function setDevice(ev) {
      if (ev) {
        ev.preventDefault();
      }

      const pin = +device_pin.value;
      const leds = +device_leds.value;
      const data = { pin, leds };

      request("POST", "/device", data);
      console.log("device updated: ", data);
      return false;
    }
    async function getRGB() {
      const res = await request("GET", "/rgb");
      console.log("rgb: ", res);
      if (!res) return;

      color_rgb.value = rgbToHex(res);
      color_w.value = res.a;
      const value = `rgb(${res.r},${res.g},${res.b},${res.a / 255})`;
      document.body.style.backgroundColor = `${value}`;
    }
    async function getWifi() {
      const res = await request("GET", "/wifi");
      console.log("wifi: ", res);
      if (!res) return;

      wifi_ssid.value = res.ssid;
      wifi_password.value = res.password;
    }
    async function getDevice() {
      const res = await request("GET", "/device");
      console.log("device: ", res);
      if (!res) return;

      device_pin.value = res.pin;
      device_leds.value = res.leds;
    }
    async function load() {
      await getWifi();
      await getDevice();
      await getRGB();
    }
    window.onload = async () => {
      await delay(2000);
      let loading = 1;
      while(loading){
        try {
          await load();
          loading = 0;
        } catch (err) {
          console.error(err);
          alert(`Try again (${loading++})!`);
        }
      }
      document.getElementById("loader").remove();
    };
  </script>
</html>
)=====";
