import csv
import matplotlib.pyplot as plt

# Input CSV file
filename = './data/Filament_log_2024_11_15_13_11_42.csv'

# Lists to hold data
tensioner_speed = []
spooler_speed = []
measured_diameter = []

# Read and parse the CSV file
with open(filename, 'r') as csvfile:
    reader = csv.reader(csvfile)
    
    for row in reader:
        # Remove the outer quotes and split by commas
        clean_row = row[0].strip('"').split(',')
        
        # Append parsed values to corresponding lists
        tensioner_speed.append(int(clean_row[1]))
        spooler_speed.append(int(clean_row[2]))
        measured_diameter.append(int(clean_row[3]))

# Plotting the data
x = range(len(tensioner_speed))  # x-axis as index (time)

plt.figure(figsize=(10, 6))

# Plot Tensioner Speed
plt.plot(x, tensioner_speed, label='Tensioner Speed', color='blue')

# Plot Spooler Speed
plt.plot(x, spooler_speed, label='Spooler Speed', color='green')

# Plot Measured Diameter
plt.plot(x, measured_diameter, label='Measured Diameter', color='red')

# Labels and legend
plt.xlabel("Time (Index)")
plt.ylabel("Speed (steps/s), Diameter (100th of mm)")
plt.title("Tensioner Speed, Spooler Speed, and Measured Diameter Over Time")
plt.legend()
plt.grid()

# Show the plot
plt.show()