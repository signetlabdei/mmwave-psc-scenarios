#!/bin/bash

# USAGE 1) Run "bash install.sh [scenario]". Available scenarios: 
# (i) chemical-plant-scenario, (ii) mva-scenario,
# (iii) high-school-shooting-scenario

# USAGE 2) Run "bash install.sh" and follow the instructions

NS3_MMWAVE_URL="https://github.com/nyuwireless-unipd/ns3-mmwave.git"
NS3_MMWAVE_IAB_URL="https://github.com/signetlabdei/ns3-mmwave-iab"

if [ $# -gt 0 ]; then
  if [ "$1" == "chemical-plant-scenario" ]; then
    choice=1
  elif [ "$1" == "mva-scenario" ]; then
    choice=2
  elif [ "$1" == "high-school-shooting-scenario" ]; then
    choice=3
  else
    echo -e "Run "bash install.sh" [scenario]\n\nAvailable scenarios:\nchemical-plant-scenario\nmva-scenario\nhigh-school-shooting-scenario\n"
    exit 1
  fi
else
  echo -e "This script will install the desired PSC scenario.\nChoose between the following:\n[1] Chemical plant explosion\n[2] Multi-vehicle accident\n[3] High school shooting\n"
  exit=false
  while [ $exit = false ]; do
    read choice
    if [ $choice = 1 ] || [ $choice = 2 ] || [ $choice = 3 ]; then
      exit=true
    else
      echo -e "Please choose 1, 2 or 3"
    fi
  done
fi

if [ $choice = 1 ]; then
  echo -e "Installing the chemical plant scenario..."

  # Clone the ns3-mmwave module
  INSTALLATION_FOLDER="chemical-plant"
  git clone -b new-handover $NS3_MMWAVE_URL $INSTALLATION_FOLDER

  # Appy the patch
  cd $INSTALLATION_FOLDER
  patch -p1 < ../.patches/ns3-mmwave-psc.patch

  # Copy the example
  cp ../psc-scenarios/chemical-plant-scenario.cc scratch/.

  # Configure the ns3-module
  ./waf configure --disable-python
  ./waf build

elif [ $choice = 2 ]; then
  echo -e "Installing the multi-vehicle accident scenario..."

  INSTALLATION_FOLDER="multi-vehicle-accident"

  # Clone the ns3-mmwave module
  git clone -b new-handover $NS3_MMWAVE_URL $INSTALLATION_FOLDER

  # Appy the patch
  cd $INSTALLATION_FOLDER
  patch -p1 < ../.patches/ns3-mmwave-psc.patch

  # Copy the example
  cp ../psc-scenarios/mva-scenario.cc scratch/.

  # Configure the ns3-module
  ./waf configure --disable-python
  ./waf build

elif [ $choice = 3 ]; then
  echo -e "Installing the high school shooting scenario..."

  INSTALLATION_FOLDER="high-school-shooting"

  # Clone the ns3-mmwave module
  git clone -b iab-release $NS3_MMWAVE_IAB_URL $INSTALLATION_FOLDER

  # Appy the patch
  cd $INSTALLATION_FOLDER
  patch -p1 < ../.patches/ns3-mmwave-iab-psc.patch

  # Copy the example
  cp ../psc-scenarios/psc-shooting-swat.cc scratch/.

  # Configure the ns3-module
  ./waf configure --disable-python
  ./waf build

else
  echo -e "Somethig went wrong.."
fi
