/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019, University of Padova, Dep. of Information Engineering, SIGNET lab.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <ns3/mmwave-helper.h>
#include <scratch/simulation-config/ps-simulation-config.h>

using namespace ns3;
using namespace mmwave;

int
main (int argc, char *argv[])
{
  uint32_t runSet = 1;
  uint32_t seedSet = 1;
  std::string filePath = ""; // path of the output files

  double frequency = 30e9; // carrier frequency in hertz
  double bandwidth = 1e9; // aggregated system bandwidth in hertz

  // APPLICATIONS
  uint32_t simTime = 1000; // simulation time in milliseconds
  uint32_t appStart = 500; // applications start time in milliseconds
  double firstRespondersVideoRate = 600e3; // data rate of the applications installed in the first responders [bit/s]
  double ratioDlFlows = 0.5; // ratio of first responders streaming a video in DL
  double packetSize = 1024; // packet size [bytes]

  // SIMULATION MODE
  bool useMmWave = true;

  // LAYOUT
  double streetWidth = 2.75 * 2; // street width in meters
  double streetLength = 25; // street length in meters

  uint32_t numInvolvedCars = 2; // number of cars involved in the incident
  double carWidth = 2.0; // car width in meters
  double carLength = 4.0; // car length in meters
  double carHeight = 1.5; // car height in meters

  uint32_t numInvolvedTrucks = 1; // number of trucks involved in the incident
  double truckWidth = 2.5; // truck width in meters
  double truckLength = 10.0; // truck length in meters
  double truckHeight = 3.0; // truck height in meters

  uint32_t numBs = 2; // number of sites
  uint32_t numSectorsPerBs = 2; // number of sectors per site
  uint32_t numLteBs = 1; // number of LTE base stations

  double interBsDistance = 50; // inter-BS distance in meters
  double lteDistanceFromScenario = 500; // m, distance of the LTE macro from the incident scene

  // UE DISTRIBUTION
  uint32_t numFirstResponders = 10; // number of first responders
  // TODO think if first responder trucks are needed
  double speed = 3; // speed of first responders

  // BS ANTENNA PARAMETERS
  uint32_t numBsAntennaElements = 64; // number of BS antenna elements
  double bsAntennaHeight = 5; // BS antenna height in meters
  double lteBsAntennaHeight = 15; // LTE BS antenna height in meters

  // UE ANTENNA PARAMETERS
  uint32_t numUeAntennaElements = 16; // number of UE antenna elements
  double ueAntennaHeight = 1.5; // UE antenna height

  // RLC PARAMETERS
  bool rlcAm = true; // if true use RLC AM, if false use RLC UM
  uint32_t rlcBufSize = 10; // RLC buffer size


  CommandLine cmd;
  cmd.AddValue ("filePath", "path for the output files", filePath);
  cmd.AddValue ("runSet", "run set", runSet);
  cmd.AddValue ("seedSet", "seed set", seedSet);
  cmd.AddValue ("simTime", "simulation time in milliseconds", simTime);
  cmd.AddValue ("appStart", "applications start time in milliseconds", appStart);
  cmd.AddValue ("firstRespondersVideoRate", "data rate of the applications installed in the first responders [bit/s]", firstRespondersVideoRate);
  cmd.AddValue ("packetSize", "packet size [bytes]", packetSize);
  cmd.AddValue ("useMmWave", "true if mmWave BSs are used", useMmWave);
  cmd.AddValue ("streetWidth", "street width in meters", streetWidth);
  cmd.AddValue ("streetLength", "street length in meters", streetLength);
  cmd.AddValue ("numInvolvedCars", "number of cars involved in the incident", numInvolvedCars);
  cmd.AddValue ("carWidth", "car width in meters", carWidth);
  cmd.AddValue ("carLength", "car length in meters", carLength);
  cmd.AddValue ("carHeight", "car height in meters", carHeight);
  cmd.AddValue ("numInvolvedTrucks", "number of trucks involved in the incident", numInvolvedTrucks);
  cmd.AddValue ("truckWidth", "truck width in meters", truckWidth);
  cmd.AddValue ("truckLength", "truck length in meters", truckLength);
  cmd.AddValue ("truckHeight", "truck height in meters", truckHeight);
  cmd.AddValue ("numFirstResponders", "number of first responders", numFirstResponders);
  cmd.AddValue ("speed", "speed of first responders", speed);
	cmd.AddValue ("numBs", "number of sites", numBs);
  cmd.AddValue ("numSectorsPerBs", "number of sectors per site", numSectorsPerBs);
  cmd.AddValue ("interBsDistance", "inter-BS distance in meters", interBsDistance);
  cmd.AddValue ("frequency", "carrier frequency in hertz", frequency);
  cmd.AddValue ("bandwidth", "aggregated system bandwidth in hertz", bandwidth);
  cmd.AddValue ("numBsAntennaElements", "number of BS antenna elements", numBsAntennaElements);
  cmd.AddValue ("bsAntennaHeight", "BS antenna height in meters", bsAntennaHeight);
  cmd.AddValue ("numUeAntennaElements", "number of UE antenna elements", numUeAntennaElements);
  cmd.AddValue ("ueAntennaHeight", "UE antenna height", ueAntennaHeight);
  cmd.AddValue ("rlcAm", "if true use RLC AM, if false use RLC UM", rlcAm);
  cmd.Parse (argc, argv);

  RngSeedManager::SetSeed (seedSet);
  RngSeedManager::SetRun (runSet);

  Config::SetDefault ("ns3::MmWavePhyMacCommon::CenterFreq", DoubleValue (frequency));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::ChunkPerRB", UintegerValue (72 * bandwidth / 1e9));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::NumRefScPerSym", UintegerValue (864 * bandwidth / 1e9));

  Config::SetDefault ("ns3::MmWaveEnbNetDevice::AntennaNum", UintegerValue (numBsAntennaElements));
  Config::SetDefault ("ns3::MmWaveUeNetDevice::AntennaNum", UintegerValue (numUeAntennaElements));

  Config::SetDefault ("ns3::MmWave3gppChannel::UpdatePeriod", TimeValue (MilliSeconds (400))); // do not update the channel during the simulation
  Config::SetDefault ("ns3::MmWave3gppChannel::DirectBeam", BooleanValue(true)); // Set true to perform the beam in the exact direction of receiver node
  Config::SetDefault ("ns3::AntennaArrayModel::IsotropicAntennaElements", BooleanValue(false)); // Use the 3gpp radiation model for the antenna elements

  Config::SetDefault ("ns3::MmWaveHelper::ChannelModel", StringValue("ns3::MmWave3gppChannel"));
  Config::SetDefault ("ns3::MmWaveHelper::PathlossModel", StringValue ("ns3::MmWave3gppBuildingsPropagationLossModel"));
  Config::SetDefault ("ns3::MmWave3gppPropagationLossModel::Scenario", StringValue ("UMa"));
  Config::SetDefault ("ns3::UdpClient::PacketSize", UintegerValue (packetSize));
  Config::SetDefault ("ns3::LteRlcAm::MaxTxBufferSize", UintegerValue (rlcBufSize * 1024 * 1024));
  Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (rlcBufSize * 1024 * 1024));
  Config::SetDefault ("ns3::LteRlcAm::PollRetransmitTimer", TimeValue(MilliSeconds(1.0)));
  Config::SetDefault ("ns3::LteRlcAm::ReorderingTimer", TimeValue(MilliSeconds(2.0)));
  Config::SetDefault ("ns3::LteRlcAm::StatusProhibitTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::LteRlcAm::ReportBufferStatusTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::LteRlcUm::ReportBufferStatusTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::MmWaveHelper::RlcAmEnabled", BooleanValue(rlcAm));

  if (!useMmWave)
  {
    Config::SetDefault ("ns3::McUePdcp::LteUplink", BooleanValue (true));
    Config::SetDefault ("ns3::McEnbPdcp::LteDownlink", BooleanValue (true));
    Config::SetDefault ("ns3::MmWave3gppChannel::UpdatePeriod", TimeValue (MilliSeconds (0))); // no need to update the mmwave channel
  }
  PsSimulationConfig::SetTracesPath (filePath); // set the path where the traces will be saved

  // Create the helpers
  Ptr<MmWaveHelper> mmWaveHelper = CreateObject<MmWaveHelper> ();
  Ptr<MmWavePointToPointEpcHelper> epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
  mmWaveHelper->SetEpcHelper (epcHelper);

  // Create the internet
  std::pair<Ptr<Node>, Ipv4Address> remoteHostPair;
  remoteHostPair = PsSimulationConfig::CreateInternet (epcHelper);

  // Create nodes for BS and UE devices
  NodeContainer bsNodes;
  bsNodes.Create(numBs);
  NodeContainer lteBsNodes;
  lteBsNodes.Create(numLteBs);

  NodeContainer firstRespondersNodes;
  firstRespondersNodes.Create(numFirstResponders);

  // LAYOUT
  // the coordinates from which the scenario is generated are (0, 0)
  // drop the cars
  for (uint8_t i = 0; i < numInvolvedCars; i++)
  {
    PsSimulationConfig::CreateRandomObstacle (streetLength, streetWidth, carWidth, carLength, carHeight);
  }

  // drop the trucks
  for (uint8_t i = 0; i < numInvolvedTrucks; i++)
  {
    PsSimulationConfig::CreateRandomObstacle (streetLength, streetWidth, truckWidth, truckLength, truckHeight);
  }

  // drop the base stations
  PsSimulationConfig::DropMmWaveRoadSideUnits(
    -interBsDistance, streetLength + interBsDistance/2, -1, bsAntennaHeight, interBsDistance, bsNodes);

  PsSimulationConfig::DropLteMacro(
    lteDistanceFromScenario, lteBsAntennaHeight, lteBsNodes
    );

  // drop the first responders
  PsSimulationConfig::DropFirstResponders (
    -streetWidth / 2, streetWidth * 1.5, 0, streetLength, ueAntennaHeight, ueAntennaHeight, firstRespondersNodes);

  BuildingsHelper::MakeMobilityModelConsistent ();

  NetDeviceContainer bsDevices = mmWaveHelper->InstallEnbDevice (bsNodes);
  NetDeviceContainer lteBsDevices = mmWaveHelper->InstallLteEnbDevice (lteBsNodes);
  NetDeviceContainer firstRespondersDevices = mmWaveHelper->InstallMcUeDevice (firstRespondersNodes);

  Ipv4InterfaceContainer ueIpIface = PsSimulationConfig::InstallUeInternet (epcHelper, firstRespondersNodes, firstRespondersDevices);

  // Connect the bs
  mmWaveHelper->AddX2Interface (lteBsNodes, bsNodes);

  mmWaveHelper->AttachToClosestEnb(firstRespondersDevices, bsDevices, lteBsDevices);

  // each first responder streams an uplink video feed
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> ulStream = asciiTraceHelper.CreateFileStream (filePath + "ul-app-trace.txt"); // trace file for UL traffic
  uint16_t ulPort = 1235; // port on the remote host to which the UL traffic is delivered
  // crate a random variable to randomize the application start times of the
  // first responders
  Ptr<UniformRandomVariable> rv = CreateObject<UniformRandomVariable> ();
  rv->SetAttribute ("Min", DoubleValue (0));
  rv->SetAttribute ("Max", DoubleValue (600));
  for (uint32_t frIndex = 0; frIndex < firstRespondersNodes.GetN (); ++frIndex)
  {
    PsSimulationConfig::SetupUplinkUdpFlow (
      firstRespondersNodes.Get(frIndex), // source
      remoteHostPair.first, // sink
      remoteHostPair.second, // sink address
      ulPort++, // port
      MicroSeconds (packetSize * 8 / firstRespondersVideoRate * 1e6), // time interval between consecutive packets [us]
      MilliSeconds (appStart + rv->GetValue ()), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      ulStream // trace
    );
  }

  NS_LOG_DEBUG ("FR applications inter-packet interval " << packetSize * 8 / firstRespondersVideoRate * 1e6);

  // some first responders watches a downlink video feed
  Ptr<OutputStreamWrapper> dlStream = asciiTraceHelper.CreateFileStream (filePath + "dl-app-trace.txt"); // trace file for DL traffic
  uint16_t dlPort = ulPort + numFirstResponders; // port on the UE to which the DL traffic is delivered
  for (uint32_t frIndex = 0; frIndex < std::floor(firstRespondersNodes.GetN () * ratioDlFlows); ++frIndex)
  {
    PsSimulationConfig::SetupUplinkUdpFlow (
      remoteHostPair.first, // source
      firstRespondersNodes.Get(frIndex), // sink
      ueIpIface.GetAddress(frIndex), // sink address
      dlPort++, // port
      MicroSeconds (packetSize * 8 / firstRespondersVideoRate * 1e6), // time interval between consecutive packets [us]
      MilliSeconds (appStart + rv->GetValue ()), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      dlStream // trace
    );
  }

  mmWaveHelper->EnableTraces ();

  PrintHelper::PrintGnuplottableBuildingListToFile ("obstacles.txt");
  PrintHelper::PrintGnuplottableNodeListToFile ("nodes.txt");

  Simulator::Stop(MilliSeconds (simTime));
  Simulator::Run();

  return 0;
}
