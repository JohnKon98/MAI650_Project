from coapthon.server.coap import CoAP
from coapthon.resources.resource import Resource
import requests
import json
import threading
GRAPHITE_HOST = '127.0.0.1'
GRAPHITE_PORT = '2003'
class CoAPtoGraphiteBridge(Resource):
   def __init__(self, name="CoAPtoGraphiteBridge", coap_server=None):
      super(CoAPtoGraphiteBridge, self).__init__(name, coap_server, visible=True,observable=True, allow_children=True)
   def render_POST(self, request):
      # Extract sensor data from the CoAP request
      sensor_data = request.payload
      # Parse the payload into a Python dictionary (assuming it's JSON)
      data_dict = json.loads(sensor_data)
      temp_value = data_dict['temperature']
      humid_value = data_dict['humidity']
      current_time = data_dict['timestamp']
      ### HERE WE SEND THE DATA with using bash command nc (-N is to close the connection after sending each payload)
      command_temp = f"echo \" MAI650_Project.temperature {temp_value} {current_time}\" | nc -N {GRAPHITE_HOST} {GRAPHITE_PORT}"
      command_humid = f"echo \"MAI650_Project.humidity {humid_value} {current_time}\" | nc -N {GRAPHITE_HOST} {GRAPHITE_PORT}"
      subprocess.run(command_temp, shell = True, executable="/bin/bash")
      subprocess.run(command_humid, shell = True, executable="/bin/bash") 
      return self
class CoAPServer(CoAP):
   def __init__(self, host, port):
     CoAP.__init__(self, (host, port))
     self.add_resource('bridge/', CoAPtoGraphiteBridge())
def main():
   server = CoAPServer("0.0.0.0", 5683)
   try:
      server.listen(10)
   except KeyboardInterrupt:
      print("Server Shutdown")
      server.close()
      print("Exiting...")
if __name__ == '__main__':
   main()
