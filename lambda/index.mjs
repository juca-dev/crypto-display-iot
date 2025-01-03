import {
  IoTDataPlaneClient,
  PublishCommand,
} from "@aws-sdk/client-iot-data-plane";

const { AWS_IOT_ENDPOINT, AWS_IOT_TOPIC } = process.env;

const client = new IoTDataPlaneClient({
  endpoint: `https://${AWS_IOT_ENDPOINT}`,
  region: "us-east-1",
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
  const res = await client.send(cmd);
  return res;
}

async function getPrice(symbol, limit = 1) {
  const url = `https://api.binance.com/api/v3/klines?symbol=${symbol}USDT&interval=1m&limit=${limit}`;
  const res = await fetch(url);
  const json = await res.json();
  console.log("json", json);
  const [, , , , closed] = json[0];

  const value = Number(closed);
  const isK = value > 1000;

  const price = new Intl.NumberFormat("en-US", {
    style: "currency",
    currency: "USD",
  }).format(isK ? value / 1000 : value);

  const display = `${symbol} ${price}${isK ? "k" : ""}`;
  return display;
}

export const handler = async (ev) => {
  const symbols = ev.symbols ?? ["BTC"];
  const time = new Date()
    .toLocaleTimeString("pt-BR", {
      timeZone: "America/Sao_Paulo",
    })
    .substr(0, 5);

  try {
    // const res = await publish(AWS_IOT_TOPIC, "CLEAR");
    const fns = symbols.map((e) => getPrice(e.toUpperCase()));

    const res = await Promise.all(fns);
    const display = `### ${time} ${res.join(" | ")}`;

    await publish(AWS_IOT_TOPIC, display);

    return {
      statusCode: 200,
      body: JSON.stringify({ display }),
    };
  } catch (err) {
    console.error("error", err);
    await publish(AWS_IOT_TOPIC, `ERR ${time}: ${err?.message}`);

    return {
      statusCode: 403,
      body: JSON.stringify(err),
    };
  }
};
