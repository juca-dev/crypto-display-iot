const {
  IoTDataPlaneClient,
  PublishCommand,
} = require("@aws-sdk/client-iot-data-plane");

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

publish(AWS_IOT_TOPIC, "Hi from lambda");
