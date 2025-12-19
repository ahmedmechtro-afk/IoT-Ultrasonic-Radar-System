# IoT-Ultrasonic-Radar-System

ESP32-based IoT Ultrasonic Radar System using HC-SR04, SG90 servo, Processing visualization, ThingSpeak, **Email Alert**, and **Buzzer Alert**.

# IoT Ultrasonic Radar System

This project is an ESP32-based IoT Ultrasonic Radar System that detects objects using an HC-SR04 ultrasonic sensor mounted on an SG90 servo motor. The system continuously scans the surroundings, visualizes detected objects on a digital radar screen, and uploads distance data to ThingSpeak for real-time monitoring.

To enhance safety and real-world usability, the system now includes **Email Alerts** and a **Buzzer Alert**. When an object is detected within a critical distance, the buzzer is activated instantly, and an automatic email notification is sent to the user.

## Features

* ESP32 microcontroller based system
* Ultrasonic distance measurement using HC-SR04
* Servo-controlled angular scanning (radar motion)
* Processing-based digital radar display
* ThingSpeak IoT cloud integration for real-time data visualization
* **Buzzer alert for nearby object detection**
* **Automatic email alert when distance threshold is crossed**

## Hardware Components

* ESP32 (ESP32-S3)
* HC-SR04 Ultrasonic Sensor
* SG90 Servo Motor
* Active/Passive Buzzer
* Breadboard and Jumper Wires

## Software Tools

* Arduino IDE
* Processing (Radar Visualization)
* ThingSpeak IoT Platform
* SMTP Email Service (for email alerts)

## Working Principle

1. The ultrasonic sensor measures the distance of objects.
2. The servo motor rotates the sensor to scan the area in multiple angles.
3. Distance and angle data are sent to the Processing application to generate a radar-like display.
4. The ESP32 uploads distance data to ThingSpeak for cloud-based visualization.
5. If an object comes within a predefined danger range:

   * The **buzzer is activated** immediately.
   * An **email alert** is sent to the registered email address.

## Block Diagram

<img width="825" height="459" alt="BLOCK DIAGRAM" src="https://github.com/user-attachments/assets/bdd0a8cd-4ea3-4f25-87fd-c46497699fb0" />

## Applications

* Obstacle detection systems
* Smart security and surveillance
* Industrial safety monitoring
* Educational IoT and embedded systems projects

## Authors

* Hafiz Saad
* Ahmed Hassan
* Obaid Ur Rehman

## University

Air University
Department of Mechatronics Engineering
Subject: ICT
Instructor: Sir Umer Farooq
