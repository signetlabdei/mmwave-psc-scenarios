# Implementation of Reference Public Safety Scenarios in ns-3

This repository contains the implementation of three reference medium-scale PSC scenarios, namely:
- chemical plant explosion
- multi-vehicle accident
- high school shooting

All the scenarios provide a communication infrastructure based on mmWave cellular networks.

For a more detailed description please refer to https://arxiv.org/abs/1902.06224.

## Installation
1. clone this repository
2. run `bash install.sh [scenario]`, where `[scenario]` specifies the scenario to install. Choose between:
   - `chemical-plant-scenario`
   - `mva-scenario`
   - `high-school-shooting-scenario`
3. The script will install the desired scenario the `[scenario]` folder
