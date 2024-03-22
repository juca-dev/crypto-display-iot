const awsIot = require("aws-iot-device-sdk");

const device = awsIot.device({
  clientId: "playbot",
  host: "a1gxnymxk1h59p-ats.iot.us-east-1.amazonaws.com",
  port: 8883,
  keyPath: "./playbot-pvt.key",
  certPath: "./playbot-cli.crt",
  caPath: "./ca.pem",
});

// We connect our client to AWS  IoT core.
device.on("connect", () => {
  console.log("Connected");
  device.subscribe("out");

  setInterval(() => {
    console.log("pub");
    return device.publish(
      "in", // topic
      JSON.stringify({ date: new Date().toISOString() }),
      { qos: 1 }
    );
  }, 5000);
});

// Set handler for the device, it will get the messages from subscribers topics.
device.on("message", (topic, payload) => {
  console.log("message", topic, payload.toString());
});

device.on("error", (topic, payload) => {
  console.log("Error:", topic, payload.toString());
});

device.on("status", function (thingName, stat, clientToken, stateObject) {
  console.log(
    "received " + stat + " on " + thingName + ": " + JSON.stringify(stateObject)
  );
  //
  // These events report the status of update(), get(), and delete()
  // calls.  The clientToken value associated with the event will have
  // the same value which was returned in an earlier call to get(),
  // update(), or delete().  Use status events to keep track of the
  // status of shadow operations.
  //
});

device.on("delta", function (thingName, stateObject) {
  console.log(
    "received delta on " + thingName + ": " + JSON.stringify(stateObject)
  );
});

device.on("timeout", function (thingName, clientToken) {
  console.log(
    "received timeout on " + thingName + " with token: " + clientToken
  );
});
