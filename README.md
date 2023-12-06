# MAI650_Project

Here you can find the 3 files we used to successfully send data to Graphite

border_router.c #is the code provided by contiki which helps us to be able to connect contiki motes with the outside world

coap_data_generator.c #is the code provided in the documentation and this is to send data to the outside world for a sensor(e.g. temperature, humidity)

graphite_send_coap.py #This is a python script that sends the data generated from contiki to Graphite in order to visualize them in real time.
