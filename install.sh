#!bin/bash
# TODO add description

NS3_MMWAVE_URL="https://github.com/nyuwireless-unipd/ns3-mmwave.git"
NS3_MMWAVE_IAB_URL="https://github.com/signetlabdei/ns3-mmwave-iab"

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

if [ $choice = 1 ]; then
  echo -e "Installing the chemical plant scenario..."

  # Clone the ns3-mmwave module
  INSTALLATION_FOLDER="chemical-plant"
  git clone -b new-handover $NS3_MMWAVE_URL $INSTALLATION_FOLDER

  # Appy the patch
  cd $INSTALLATION_FOLDER
  patch -p1 < ../.patches/chemical-plant-scenario.patch

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
  patch -p1 < ../.patches/mva-scenario.patch

  # Configure the ns3-module
  ./waf configure --disable-python
  ./waf build

elif [ $choice = 3 ]; then
  echo -e "Installing the high school shooting scenario..."

  INSTALLATION_FOLDER="high-school-shooting"

  # Clone the ns3-mmwave module
  git clone -b new-handover $NS3_MMWAVE_IAB_URL $INSTALLATION_FOLDER

  # Appy the patch
  cd $INSTALLATION_FOLDER
  patch -p1 < ../.patches/high-school-shooting.patch

  # Configure the ns3-module
  ./waf configure --disable-python
  ./waf build

else
  echo -e "Somethig went wrong.."
fi
