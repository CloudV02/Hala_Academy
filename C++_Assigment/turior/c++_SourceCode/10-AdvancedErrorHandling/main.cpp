#include "TemperatureSensor.h"
#include "SensorError.h"
#include <iostream>
#include <limits>

using namespace std;

void displayMenu()
{
    cout << "\n--- Temperature Sensor Menu ---\n";
    cout << "1. Initialize Sensor\n";
    cout << "2. Read Temperature\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    TemperatureSensor sensor;
    int choice = 0;
    double temp = 0.0f;

    do
    {
        displayMenu();
        cin >> choice;

        try
        {
            switch (choice)
            {
                case 1:
                    sensor.initialize();
                    cout << "Sensor initialized successfully.\n";
                    break;

                case 2:
                    temp = sensor.readTemperature();
                    cout << "Temperature: " << temp << " °C\n";
                    break;

                case 3:
                    cout << "Exiting...\n";
                    break;

                default:
                    cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            }
        }
        catch (const SensorNotInitialized& e)
        {
            cout << "Initialization Error: " << e.what() << endl;
        }
        catch (const SensorReadFailure& e)
        {
            cout << "Read Error: " << e.what() << endl;
        }
        catch (const SensorError& e)
        {
            cout << "Sensor Error: " << e.what() << endl;
        }
        catch (const exception& e)
        {
            cout << "Unexpected Error: " << e.what() << endl;
        }
    }
    while (choice != 3);

    return 0;
}
