#include <pgmspace.h>
 
// #define SECRET
#define THINGNAME "playbot"                         //change this
 
const char WIFI_SSID[] = "JUCA";               //change this
const char WIFI_PASSWORD[] = "curitiba2808";           //change this
const char AWS_IOT_ENDPOINT[] = "a1gxnymxk1h59p-ats.iot.us-east-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUIkdK0Dqp6bkhiVV9HUTa4DLTOwgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDMyMDE3MzI1
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKsIr089C/FuCEDXVjBP
1jCEifF8SJ/gThDg/+CUK6+Qfru172wOtKWKXC+Rqo9BoBRxHh/enkxwRVB4j+ST
ioCzzqNQbaZ34KDHo8eSxq33Kf42O3OW234eByIA3dtcWsY/JLTuGTP9G8SQJcGq
F7y9gZnxqWcgDzgKwYwgbjLvGTKrYma8fT1gxzC0Dg0S5rRG5acsPJTtlq/hd/8w
2Y9v0JzbhJJuSP5UYbM7oijuajHbfJrtKKBSc9TJeIDmS9Bv+HFbVwNN6BPKVI9l
ituzzie7N56oFPUa//lpFI4VIiTD0eg7W1Fee/jxsSzzgaYPB4JqLv6/loiHWEY1
EvsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUdOD7mFOZ29ADLbAzZzXu2rXoHHkwHQYD
VR0OBBYEFG8/JyoDDhwUel4aQ3UY+fdRwrPUMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAxpu1LfC9zMHBggrsoBJjECjEl
VVfYycVkwMmDN9lrUGU72S6OkKkBNiTCf+Flk8xx61WaHbKWq60IUgxs1QEhElM9
di2UlAYw54ibMtdQuNERXS/7MLSlTwtM6b6TQVVL122bZoTC0sFbd2E7zJgfZ7+7
85aScyg+Dcl6gEd96AF3pci4K36PlmdPyFFadOZbpWHwOC39hcyfRHfrIMVpM6Xp
5O0ej0tsYFiVFoUvXEhxZO6CrnXATAyUQQB33c2mklRgy79Xgp8qO5TGtiro33aF
EB7t0XD1NLYs4VCzWFgz/Q3Tzqs+X61aus/86P6lKMOLzlWRVzKAh1U5rbRG
-----END CERTIFICATE-----
)EOF";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAqwivTz0L8W4IQNdWME/WMISJ8XxIn+BOEOD/4JQrr5B+u7Xv
bA60pYpcL5Gqj0GgFHEeH96eTHBFUHiP5JOKgLPOo1BtpnfgoMejx5LGrfcp/jY7
c5bbfh4HIgDd21xaxj8ktO4ZM/0bxJAlwaoXvL2BmfGpZyAPOArBjCBuMu8ZMqti
Zrx9PWDHMLQODRLmtEblpyw8lO2Wr+F3/zDZj2/QnNuEkm5I/lRhszuiKO5qMdt8
mu0ooFJz1Ml4gOZL0G/4cVtXA03oE8pUj2WK27POJ7s3nqgU9Rr/+WkUjhUiJMPR
6DtbUV57+PGxLPOBpg8Hgmou/r+WiIdYRjUS+wIDAQABAoIBAQCA87dZwukySoem
e7TcEBWp1wQ6Mj5/bBrv1vSVWqHlaMhktrOhgSTqevsuUmGKSSxsvSy9Upd/glaA
aaXz7NnMV8VAlO9XR6LxFhSB8EQj/eqHZFRGvkMXP4SZOpYLQgKe3hTK5YnTDM7W
0OfmsFvdTfRdITtcKWvQESwEfp8XfrpAphuFWgjDi+G9Y0UZpGduId/sgI9pbBNm
lUynP19FeBA9EBQtriVdYI2qwSI0+qHo3zzCtPL7fLyLkLYTUIDL/z3462DL+hmJ
k0QDoYpGPE++0z3ETbiKMLjr8CPJkwplKMNvDAGkz8JmclKeHfsB1PdbMQs8DJ5p
5JOgnl9hAoGBANzEEG2tbYTu7uWtyBSThd+xwOyZnLED5aABTXTNbBD7SNVfKcAc
nAUD5nEqEjs0MaSROPQd4cGLY6h476iSXsyQgwTItM9dRWcxmSx4F9JHn3x3pexA
ynVTMZVTn+WzZCD7sUMKJxSWUWPLn6mNo7MZxaCNcbrmeQ1EHXJeD3f1AoGBAMZU
suQV5NqErOqFB8XY6eKAlDML2979azSHSErAI/RXxE6V/xoU3SE00aV1Fi+yFekR
TcPE1+U+1mxRe4LG+Z85Sza4SjezKDPwJ9fqrWnJvYnlHtMBdhqwuQK2+yY5bPKx
up/SCqgrWH3LcEfnrQQpRNuG56JNk+KSy8SuNrkvAoGAFHbMOw53d3oahQnd4SJn
p3u3sXbv0mzVDUbxoiEkolJYxwaiPR+pysE4zEalF/3XmDSp/d8DPUipfUVwmcSv
qvC+ZGQidpJjy5QKadfcBW8ENqp0o/jBi6yhjQEKcmV1o6URInC/fKYOd1y5uSLb
CNAbxK033KZelD87J43Wiz0CgYBrvkms1SQNfjQ992Ugm1V8AiQAkzmwK3XZBIFG
cplVvXRcyOncLmdvCbW2sCQMZy+1/PuFFeO/m2J7V8zEX/nuAP8WTAmttrtpc9cB
WnE43isExZ74PHv/xe/MRMGto1bHhbv/5Oh7MYkSVbm6pmZ8szrAE8q0hnaHiP3/
7DC4awKBgQDbibb4yxkrgmtswHlS8pIdmahYGm1GCzTFgHcVZpQ+TR3tXq88joCK
vv/279SPFpXuJZsnjQcWXv0xRqeIL3sOn0fLPMkFAMXeq6XO6vLUhgLkWiVJY3tm
7ODgcd57YMhUpPvxYA2ct7d2LpbLd8zW7Jg3FqST/FBSU7/ev1CBhA==
-----END RSA PRIVATE KEY-----
)EOF";