import requests
from bs4 import BeautifulSoup
import numpy as np
import csv
#to do - save data, extract data from html
# import matplotlib.pyplot as plt


num_requests = 20
while(num_requests > 0):
# for i in range(1,20):
	r = requests.get("http://192.168.1.100/")
	#192.168.5.206

	soup = BeautifulSoup(r.content)
	data_sensor = soup.prettify()

	print("------------------------------------------")
	print(data_sensor)
	# print("")
	# newdata = np.squeeze(data_sensor) # Shape is now: (10, 80)
	# plt.plot(newdata) # plotting by columns
	# plt.show()
	print("------------------------------------------")


	
	pass
	table = data_sensor.find(id='curr_table')
	table = table.find_all('td')

	row_data = []
	for row in table:
		row_data.append(row.get_text('data-real-value'))

	with open(save_file, 'w') as save:
		for row in row_data:
			writer = csv.writer(save, delimiter=';')
			writer.writerow(row)

getData(save_file="sensor_data.csv")
# result = s.recv(4096)

