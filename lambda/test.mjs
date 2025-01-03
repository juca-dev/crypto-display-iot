import { handler } from "./index.mjs";

process.env.AWS_IOT_ENDPOINT = "a1gxnymxk1h59p-ats.iot.us-east-1.amazonaws.com";
process.env.AWS_IOT_TOPIC = "playbot/in";

handler({ symbols: ["BRL", "DOGE"] })
  .then((e) => console.log(e))
  .catch((err) => console.error(err));
