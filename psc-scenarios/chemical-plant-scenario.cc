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
  uint32_t simTime = 1000; // simulation time in milliseconds

  double frequency = 30e9; // operating frequency
  double bandwidth = 1e9; // system bandwidth

  // APPLICATIONS
  uint32_t wheelbarrowAppStart = 500; // wheelbarrow robot applications start time in milliseconds
  uint32_t firstRespondersAppStart = 500; // first responders applications start time in milliseconds
  double firstRespondersVideoRate = 600e3; // data rate of the applications installed in the first responders [bit/s]
  double ratioDlFlows = 0.5; // ratio between UL and DL flows
  double wheelbarrowVideoRate = 4000e3; // data rate of the video application installed in the wheelbarrow robot [bit/s]
  double wheelbarrowCtrlRate = 50e3; // data rate of the application which controls the wheelbarrow robot [bit/s]
  bool wheelbarrowCtrlOnLte = true; // if true, send the traffic to control the wheelbarrow robot through LTE
  double packetSize = 1024; // packet size [bytes]

  // LAYOUT
  double incidentAreaRadius = 200.0; // radius of the incident area in meters
  double plantSide = 1000.0; // dimension of the chemical plant in meters
  double minBuildingSize = 50.0; // minimum dimension of a building in meters
  double maxBuildingSize = 300.0; // maximum dimension of a building in meters
  uint32_t numOfBuildings = 10; // number of buildings

  uint32_t numOfMmWaveBs = 5; // number of mmwave base stations
  double minInterSiteDistance = 60.0; // minimum intersite distance between the bs

  uint32_t numFirstResponders = 10; // number of first responders

  // BS ANTENNA PARAMETERS
  uint32_t numBsAntennaElements = 64; // number of BS antenna elements
  double mmWaveBsHeight = 10.0; // mmwave site height
  double lteBsHeight = 30.0; // LTE bs height

  // UE ANTENNA PARAMETERS
  uint32_t numUeAntennaElements = 16; // number of UE antenna elements
  double ueAntennaHeight = 1.5; // UE antenna height

  // RLC PARAMETERS
  bool rlcAm = true; // if true use RLC AM, if false use RLC UM
  uint32_t rlcBufSize = 10; // RLC buffer size

  CommandLine cmd;
  cmd.AddValue ("filePath", "path for the output files", filePath);
  cmd.AddValue ("simTime", "simulation time in milliseconds", simTime);
  cmd.AddValue ("runSet", "run set", runSet);
  cmd.AddValue ("seedSet", "seed set", seedSet);
  cmd.AddValue ("wheelbarrowAppStart", "wheelbarrow robot applications start time in milliseconds", wheelbarrowAppStart);
  cmd.AddValue ("firstRespondersAppStart", "first responders applications start time in milliseconds", firstRespondersAppStart);
  cmd.AddValue ("firstRespondersVideoRate", "data rate of the applications installed in the first responders [bit/s]", firstRespondersVideoRate);
  cmd.AddValue ("wheelbarrowVideoRate", "data rate of the video application installed in the wheelbarrow robot [bit/s]", wheelbarrowVideoRate);
  cmd.AddValue ("wheelbarrowCtrlRate", "data rate of the application which controls the wheelbarrow robot [bit/s]", wheelbarrowCtrlRate);
  cmd.AddValue ("wheelbarrowCtrlOnLte", "if true, send the traffic to control the wheelbarrow robot through LTE", wheelbarrowCtrlOnLte);
  cmd.AddValue ("packetSize", "packet size [bytes]", packetSize);
  cmd.AddValue ("incidentAreaRadius", "radius of the incident area in meters", incidentAreaRadius);
  cmd.AddValue ("plantSide", "dimension of the chemical plant in meters", plantSide);
  cmd.AddValue ("minBuildingSize", "minimum dimension of a building in meters", minBuildingSize);
  cmd.AddValue ("maxBuildingSize", "maximum dimension of a building in meters", maxBuildingSize);
  cmd.AddValue ("numOfMmWaveBs", "number of mmwave base stations", numOfMmWaveBs);
  cmd.AddValue ("mmWaveBsHeight", "height of the mmwave base stations", mmWaveBsHeight);
  cmd.AddValue ("lteBsHeight", "height of the lte base stations", lteBsHeight);
  cmd.AddValue ("minInterSiteDistance", "minimum intersite distance between the bs", minInterSiteDistance);
  cmd.AddValue ("numFirstResponders", "number of first responders", numFirstResponders);
  cmd.AddValue ("ueAntennaHeight", "UE antenna height", ueAntennaHeight);
  cmd.AddValue ("ratioDlFlows", "ratio between UL and DL flows", ratioDlFlows);
  cmd.AddValue ("numBsAntennaElements", "number of BS antenna elements", numBsAntennaElements);
  cmd.AddValue ("numUeAntennaElements", "number of UE antenna elements", numUeAntennaElements);
  cmd.AddValue ("rlcAm", "if true use RLC AM, if false use RLC UM", rlcAm);
  cmd.AddValue ("rlcBufSize", "RLC buffer size", rlcBufSize);
  cmd.AddValue ("frequency", "operating frequency", frequency);
  cmd.AddValue ("bandwidth", "system bandwidth", bandwidth);
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

  // Do not perform handover to lte, use always the mmwave connection.
  // Use the LTE connection only for the traffic to control the wheelbarrow
  // robot if wheelbarrowCtrlOnLte is set to true
  Config::SetDefault ("ns3::LteEnbRrc::OutageThreshold", DoubleValue (-10000.0));

  NodeContainer lteBsNode;
  lteBsNode.Create (1);

  NodeContainer mmWaveBsNodes;
  mmWaveBsNodes.Create (numOfMmWaveBs);

  NodeContainer firstRespondersNodes;
  firstRespondersNodes.Create (numFirstResponders);

  NodeContainer wheelbarrowRobotNode;
  wheelbarrowRobotNode.Create (1);

  // create the buildings
  RandomBuildings::CreateRandomBuildings (2.0,              // street width between buildings
                                          minBuildingSize,  // min building side
                                          maxBuildingSize,  // max building size
                                          plantSide,        // x-side of the deployment area
                                          plantSide,        // y-side of the deployment area
                                          numOfBuildings);  // number of buldings

  //drop the LTE macro
  PsSimulationConfig::SetConstantPositionMobility (lteBsNode, Vector (0.0, 0.0, lteBsHeight));

  // drop the mmaWave base stations
  // random deployment outdoor and outside the incident area with a constraint
  // on the minimum inter site distance
  PsSimulationConfig::DropMmWaveRandomUnits (mmWaveBsNodes,            // nodes
                                           plantSide,          // xMax
                                           plantSide,          // yMax
                                           mmWaveBsHeight,           // site height
                                           incidentAreaRadius,  // forbidden radius
                                           minInterSiteDistance); // minimum intersite distance between the bs
                                           PsSimulationConfig::SetTracesPath (filePath); // set the path where the traces will be saved

  // drop the first responders
  PsSimulationConfig::DropFirstResponders (0,
                                         plantSide,
                                         0,
                                         plantSide,
                                         ueAntennaHeight,
                                         ueAntennaHeight,
                                         firstRespondersNodes);

  // drop the wheelbarrow robot inside the incident area
  PsSimulationConfig::DropWheelbarrowRobot (plantSide / 2 - incidentAreaRadius,
                                          plantSide / 2 + incidentAreaRadius,
                                          plantSide / 2 - incidentAreaRadius,
                                          plantSide / 2 + incidentAreaRadius,
                                          ueAntennaHeight,
                                          ueAntennaHeight,
                                          wheelbarrowRobotNode);

  BuildingsHelper::MakeMobilityModelConsistent ();

  // Create the helpers
  Ptr<MmWaveHelper> mmWaveHelper = CreateObject<MmWaveHelper> ();
  Ptr<MmWavePointToPointEpcHelper> epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
  mmWaveHelper->SetEpcHelper (epcHelper);

  // Create the internet
  std::pair<Ptr<Node>, Ipv4Address> remoteHostPair;
  remoteHostPair = PsSimulationConfig::CreateInternet (epcHelper);

  // Install the devices
  NetDeviceContainer lteBsDevice = mmWaveHelper->InstallLteEnbDevice (lteBsNode);
  NetDeviceContainer mmWaveBsDevices = mmWaveHelper->InstallEnbDevice (mmWaveBsNodes);
  NetDeviceContainer ueDevices = mmWaveHelper->InstallMcUeDevice (NodeContainer(firstRespondersNodes, wheelbarrowRobotNode));

  // Install the IP stack on the UEs
  Ipv4InterfaceContainer ueIpIface = PsSimulationConfig::InstallUeInternet (epcHelper, NodeContainer (firstRespondersNodes, wheelbarrowRobotNode), ueDevices);

  // Connect the bs
  mmWaveHelper->AddX2Interface (lteBsNode, mmWaveBsNodes);

  // Attach the UEs to the closest BSs
  mmWaveHelper->AttachToClosestEnb(ueDevices, mmWaveBsDevices, lteBsDevice);

  // each first responder streams an uplink video feed
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> ulStream = asciiTraceHelper.CreateFileStream (filePath + "fr-ul-video-stream.txt"); // trace file for UL traffic
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
      MilliSeconds (firstRespondersAppStart + rv->GetValue ()), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      ulStream // trace
    );
  }

  // some first responders watches a downlink video feed
  Ptr<OutputStreamWrapper> dlStream = asciiTraceHelper.CreateFileStream (filePath + "fr-dl-video-stream.txt"); // trace file for DL traffic
  uint16_t dlPort = ulPort + numFirstResponders + 1; // port on the UE to which the DL traffic is delivered
  for (uint32_t frIndex = 0; frIndex < std::floor(firstRespondersNodes.GetN () * ratioDlFlows); ++frIndex)
  {
    PsSimulationConfig::SetupUplinkUdpFlow (
      remoteHostPair.first, // source
      firstRespondersNodes.Get(frIndex), // sink
      ueIpIface.GetAddress(frIndex), // sink address
      dlPort++, // port
      MicroSeconds (packetSize * 8 / firstRespondersVideoRate * 1e6), // time interval between consecutive packets [us]
      MilliSeconds (firstRespondersAppStart + rv->GetValue ()), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      dlStream // trace
    );
  }

  NS_LOG_DEBUG ("First responders applications interPacketInterval " << packetSize * 8 / firstRespondersVideoRate * 1e6 << " us");

  // the wheelbarrow robot streams an uplink video feed to the incident command
  ulStream = asciiTraceHelper.CreateFileStream (filePath + "wb-ul-video-stream.txt"); // trace file for UL traffic
  uint16_t wbUlVideoPort = 1233;
  PsSimulationConfig::SetupUplinkUdpFlow (
      wheelbarrowRobotNode.Get(0), // source
      remoteHostPair.first, // sink
      remoteHostPair.second, // sink address
      wbUlVideoPort, // port
      MicroSeconds (packetSize * 8 / wheelbarrowVideoRate * 1e6), // time interval between consecutive packets [us]   // TODO the wheelbarrow streams an HQ video
      MilliSeconds (wheelbarrowAppStart), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      ulStream // trace
    );

  NS_LOG_DEBUG ("Wheelbarrow video application interPacketInterval " << packetSize * 8 / wheelbarrowVideoRate * 1e6 << " us");

  // the incident command remotely controls the wheelbarrow robot
  dlStream = asciiTraceHelper.CreateFileStream (filePath + "wb-ctrl-traffic.txt"); // trace file for DL traffic
  uint16_t wbDlCtrlPort = 1234;
  PsSimulationConfig::SetupUplinkUdpFlow (
      remoteHostPair.first, // source
      wheelbarrowRobotNode.Get(0), // sink
      ueIpIface.GetAddress(numFirstResponders), // sink address
      wbDlCtrlPort, // port
      MicroSeconds (packetSize * 8 / wheelbarrowCtrlRate * 1e6), // time interval between consecutive packets [us]
      MilliSeconds (wheelbarrowAppStart), // start time of the application [ms]
      MilliSeconds (simTime - 100), // end time of the application [ms]
      dlStream // trace
    );

  NS_LOG_DEBUG ("Wheelbarrow contrl application interPacketInterval " << packetSize * 8 / wheelbarrowCtrlRate * 1e6 << " us");

  if (wheelbarrowCtrlOnLte)
  {
    // Create a dedicated high-priority bearer for the wheelbarrow robot control
    // traffic. High-priority bearers (priority level < 10) are always sent
    // through LTE
    Ptr<NetDevice> wbDevice = ueDevices.Get (numFirstResponders);
    PsSimulationConfig::SetupDedicatedBearer (wbDevice, wbDlCtrlPort, epcHelper, EpsBearer::NGBR_MC_DELAY_SENSI_SIG);
  }

  mmWaveHelper->EnableTraces ();

  PrintHelper::PrintGnuplottableBuildingListToFile ("obstacles.txt");
  PrintHelper::PrintGnuplottableNodeListToFile ("nodes.txt");

  Simulator::Stop(MilliSeconds (simTime));
  Simulator::Run();

  return 0;
}
