const {
  IoTDataPlaneClient,
  PublishCommand,
} = require("@aws-sdk/client-iot-data-plane");
const { resolve } = require("styled-jsx/css");

const {
  AWS_REGION,
  AWS_ACCESS_KEY_ID,
  AWS_SECRET_ACCESS_KEY,
  AWS_SESSION_TOKEN,
  AWS_IOT_ENDPOINT,
  AWS_IOT_TOPIC,
} = process.env;

const client = new IoTDataPlaneClient({
  region: AWS_REGION,
  endpoint: `https://${AWS_IOT_ENDPOINT}`,
  credentials: {
    accessKeyId: AWS_ACCESS_KEY_ID,
    secretAccessKey: AWS_SECRET_ACCESS_KEY,
    sessionToken: AWS_SESSION_TOKEN,
  },
});

async function publish(topic, data) {
  const payload = {
    data,
  };

  const params = {
    topic,
    payload: JSON.stringify(payload),
    qos: 0,
  };
  const cmd = new PublishCommand(params);
  try {
    const data = await client.send(cmd);
    console.log("sent", data);
  } catch (err) {
    console.log("error", err);
  }
}

async function main(symbol = "BTC") {
  publish(AWS_IOT_TOPIC, "CLEAR");

  while (true) {
    const time = new Date().toTimeString().substring(0, 8);

    try {
      const res = await fetch(
        `https://api.binance.com/api/v3/klines?symbol=${symbol}USDT&interval=1m&limit=${1}`
      );
      const json = await res.json();
      const [, , , , closed] = json[0];

      const price = new Intl.NumberFormat("en-US", {
        style: "currency",
        currency: "USD",
      }).format(Number(closed));

      const display = `${symbol} ${price} ${time}`;
      publish(AWS_IOT_TOPIC, display);

      await new Promise((resolve) => setTimeout(resolve, 30000));
    } catch (err) {
      console.error(err);
      publish(AWS_IOT_TOPIC, `ERR ${time}`);
    }
  }
}

main().then(console.log).catch(console.error);
