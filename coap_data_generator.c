#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/log.h"
#include <stdio.h>
#include "random.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP
#define SERVER_EP "coap://[fd00::302:304:506:708]:5683"
#define INTERVAL (60 * CLOCK_SECOND)
static coap_endpoint_t server_ep;
static coap_message_t request[1]; // This way the packet can be re-used
PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);
static void response_callback(coap_message_t *response) {
const uint8_t *chunk;
if(response == NULL) {
puts("Request timed out");
return;
}
int len = coap_get_payload(response, &chunk);
printf("|%.*s", len, (char *)chunk);
}
PROCESS_THREAD(coap_client_example, ev, data)
{
static struct etimer timer;
PROCESS_BEGIN();
coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
etimer_set(&timer, INTERVAL);
while(1) {
   PROCESS_YIELD();
   if(etimer_expired(&timer)) {
     // Prepare the message
     coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
     coap_set_header_uri_path(request, "COAP_DATA:");
     // Generate random temperature and humidity
     static unsigned int temperature;
     static unsigned int humidity;
     temperature = (unsigned int)random_rand() % 30;
     humidity = (unsigned int)random_rand() % 100;
     char msg[64];
     snprintf(msg, sizeof(msg), "{\"temperature\": %u, \"humidity\": %u}", temperature,humidity);
     coap_set_payload(request, (uint8_t *)msg, sizeof(msg));
     COAP_BLOCKING_REQUEST(&server_ep, request, response_callback);
     etimer_reset(&timer);
   }
}
PROCESS_END();
}
