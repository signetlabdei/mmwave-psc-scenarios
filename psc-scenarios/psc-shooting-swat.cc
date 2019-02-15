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

  // APPLICATIONS
  uint32_t appStart = 500; // applications start time in milliseconds
  double officersVideoRate = 100e6; // data rate of the applications installed in the first responders [bit/s]
  double packetSize = 1024; // packet size [bytes]

  // LAYOUT
  double corridorWidth = 3.5; // corridor width in meters
  double roomWidthX = 20; // room width in the x axis meters
  double roomWidthY = 20; // room width in the y axis meters
  double roomHeight = 3; // room height in meters
  int numRoomsRow = 4; // number of rooms in the x axis
  int numRoomsColumn = 4; // number of rooms in the y axis

  // UE DISTRIBUTION
  uint32_t numSwatTeams = 4; // number of swat teams
  uint32_t numOfficersPerTeam = 4; // number of officers per team
  double swatTeamMaxSpeed = 5; // maximum speed of the SWAT teams [m/s]
  double swatTeamMinSpeed = 3; // minimum speed of the SWAT teams [m/s]

  uint32_t numBs = 4; // number of sites
  uint32_t numSectorsPerBs = 1; // number of sectors per site

  double frequency = 30e9; // carrier frequency in hertz
  double bandwidth = 400e6; // aggregated system bandwidth in hertz

  // BS ANTENNA PARAMETERS
  uint32_t numBsAntennaElements = 64; // number of BS antenna elements
  double bsAntennaHeight = 2.5; // BS antenna height in meters
  double iabAntennaHeight = 1.5; // IAB BS antenna height in meters (on SWAT officer)

  // UE ANTENNA PARAMETERS
  uint32_t numUeAntennaElements = 16; // number of UE antenna elements
  double ueAntennaHeight = 1.5; // UE antenna height

  // RLC PARAMETERS
  bool rlcAm = true; // if true use RLC AM, if false use RLC UM
  uint32_t rlcBufSize = 10; // RLC buffer size

  bool useIab = true; // if true, the SWAT team leader carries an IAB rely

  uint32_t mobilityMode = 1; // with mobility mode 1 the IAB follows the team,
                             // with mobility mode 2 the IAB stops before the
                             // corner in such a way to be always LOS with the
                             // donor

  CommandLine cmd;
  cmd.AddValue ("filePath", "path for the output files", filePath);
  cmd.AddValue ("runSet", "run set", runSet);
  cmd.AddValue ("seedSet", "seed set", seedSet);
  cmd.AddValue ("appStart", "applications start time in milliseconds", appStart);
  cmd.AddValue ("officersVideoRate", "data rate of the applications installed in the first responders [bit/s]", officersVideoRate);
  cmd.AddValue ("packetSize", "packet size [bytes]", packetSize);
  cmd.AddValue ("corridorWidth", "corridor width in meters", corridorWidth);
  cmd.AddValue ("roomWidthX", "room width in the x axis meters", roomWidthX);
  cmd.AddValue ("roomWidthY", "room width in the y axis meters", roomWidthY);
  cmd.AddValue ("roomHeight", "room height meters", roomHeight);
  cmd.AddValue ("numRoomsRow", "number of rooms in each row", numRoomsRow);
  cmd.AddValue ("numRoomsColumn", "number of rooms in each column", numRoomsColumn);
  cmd.AddValue ("numSwatTeams", "number of swat teams", numSwatTeams);
  cmd.AddValue ("numOfficersPerTeam", " number of officers per team", numOfficersPerTeam);
	cmd.AddValue ("numBs", "number of sites", numBs);
  cmd.AddValue ("numSectorsPerBs", "number of sectors per site", numSectorsPerBs);
  cmd.AddValue ("frequency", "carrier frequency in hertz", frequency);
  cmd.AddValue ("bandwidth", "aggregated system bandwidth in hertz", bandwidth);
  cmd.AddValue ("numBsAntennaElements", "number of BS antenna elements", numBsAntennaElements);
  cmd.AddValue ("bsAntennaHeight", "BS antenna height in meters", bsAntennaHeight);
  cmd.AddValue ("iabAntennaHeight", "IAB BS antenna height in meters ", iabAntennaHeight);
  cmd.AddValue ("numUeAntennaElements", "number of UE antenna elements", numUeAntennaElements);
  cmd.AddValue ("ueAntennaHeight", "UE antenna height", ueAntennaHeight);
  cmd.AddValue ("swatTeamMinSpeed", "minimum speed of the SWAT teams [m/s]", swatTeamMinSpeed);
  cmd.AddValue ("swatTeamMaxSpeed", "maximum speed of the SWAT teams [m/s]", swatTeamMaxSpeed);
  cmd.AddValue ("rlcAm", "if true use RLC AM, if false use RLC UM", rlcAm);
  cmd.AddValue ("useIab", "if true, the SWAT team leader carries an IAB rely", useIab);
  cmd.AddValue ("mobilityMode", "with mobility mode 1 the IAB follows the team, with mobility mode 2 the IAB stops before the corner in such a way to be always LOS with the donor", mobilityMode);
  cmd.Parse (argc, argv);

  RngSeedManager::SetSeed (seedSet);
  RngSeedManager::SetRun (runSet);

  NS_ABORT_MSG_IF(numBs > 4, "Too many wired BSs");
  NS_ABORT_MSG_IF(numRoomsRow % 2 != 0 || numRoomsColumn % 2 != 0, "Please use an even number of rooms");

  Config::SetDefault ("ns3::MmWavePhyMacCommon::CenterFreq", DoubleValue (frequency));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::ChunkPerRB", UintegerValue (72 * bandwidth / 1e9));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::NumRefScPerSym", UintegerValue (864 * bandwidth / 1e9));

  Config::SetDefault ("ns3::MmWaveEnbNetDevice::AntennaNum", UintegerValue (numBsAntennaElements));
  Config::SetDefault ("ns3::MmWaveUeNetDevice::AntennaNum", UintegerValue (numUeAntennaElements));
  Config::SetDefault ("ns3::MmWaveIabNetDevice::AccessAntennaNum", UintegerValue (numBsAntennaElements));
  Config::SetDefault ("ns3::MmWaveIabNetDevice::BackhaulAntennaNum", UintegerValue (numBsAntennaElements));

  Config::SetDefault ("ns3::MmWaveHelper::ChannelModel", StringValue ("ns3::MmWave3gppChannel"));
  Config::SetDefault ("ns3::MmWaveHelper::PathlossModel", StringValue ("ns3::MmWave3gppBuildingsPropagationLossModel"));
  Config::SetDefault ("ns3::MmWave3gppChannel::UpdatePeriod", TimeValue (MilliSeconds (100)));
  Config::SetDefault ("ns3::MmWave3gppPropagationLossModel::Scenario", StringValue ("InH-OfficeMixed"));

  Config::SetDefault ("ns3::UdpClient::PacketSize", UintegerValue (packetSize));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::SymbolsPerSubframe", UintegerValue(240));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::SubframePeriod", DoubleValue(1000));
  Config::SetDefault("ns3::MmWavePhyMacCommon::UlSchedDelay", UintegerValue(1));
  Config::SetDefault ("ns3::LteRlcAm::MaxTxBufferSize", UintegerValue (rlcBufSize * 1024 * 1024));
  Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (rlcBufSize * 1024 * 1024));
  Config::SetDefault ("ns3::LteRlcAm::PollRetransmitTimer", TimeValue(MilliSeconds(1.0)));
  Config::SetDefault ("ns3::LteRlcAm::ReorderingTimer", TimeValue(MilliSeconds(2.0)));
  Config::SetDefault ("ns3::LteRlcAm::StatusProhibitTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::LteRlcAm::ReportBufferStatusTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::LteRlcUm::ReportBufferStatusTimer", TimeValue(MicroSeconds(500)));
  Config::SetDefault ("ns3::MmWaveHelper::RlcAmEnabled", BooleanValue(rlcAm));
  Config::SetDefault ("ns3::MmWaveFlexTtiMacScheduler::CqiTimerThreshold", UintegerValue(100));

  PsSimulationConfig::SetTracesPath (filePath); // set the path where the traces will be saved

  // Create the helpers
  Ptr<MmWaveHelper> mmWaveHelper = CreateObject<MmWaveHelper> ();
  Ptr<MmWavePointToPointEpcHelper> epcHelper = CreateObject<MmWavePointToPointEpcHelper> ();
  mmWaveHelper->SetEpcHelper (epcHelper);

  // Create the internet
  std::pair<Ptr<Node>, Ipv4Address> remoteHostPair;
  remoteHostPair = PsSimulationConfig::CreateInternet (epcHelper);

  // Create nodes for IAB-nodes, IAB-donors and UE devices
  NodeContainer fixedWiredBackhaulNodes;
  fixedWiredBackhaulNodes.Create(numBs);

  NodeContainer iabNodesForSwatTeams;
  iabNodesForSwatTeams.Create(numSwatTeams);

  std::vector<NodeContainer> swatTeams;
  NodeContainer allOfficers;
  for (uint32_t swIndex = 0; swIndex < numSwatTeams; ++swIndex)
  {
    NodeContainer swatTeamMembers;
    swatTeamMembers.Create(numOfficersPerTeam);
    allOfficers.Add(swatTeamMembers);
    swatTeams.push_back(swatTeamMembers);
  }

  // LAYOUT
  // deploy the rooms as buildings
  double buildingSizeX = numRoomsRow * roomWidthX + (numRoomsRow - 1) * corridorWidth;
  double buildingSizeY = numRoomsColumn * roomWidthY + (numRoomsColumn - 1) * corridorWidth;
  double halfBuildingCorridorRow = std::ceil(numRoomsRow / 2) * (roomWidthX + corridorWidth) + corridorWidth/2;
  double halfBuildingCorridorColumn = std::ceil(numRoomsRow / 2) * (roomWidthX + corridorWidth) + corridorWidth/2;
  Ptr<UniformRandomVariable> randomSpeed = CreateObject<UniformRandomVariable>();
  randomSpeed->SetAttribute ("Min", DoubleValue (swatTeamMinSpeed));
  randomSpeed->SetAttribute ("Max", DoubleValue (swatTeamMaxSpeed));
  // maximum time it takes to reach the center after the application has started
  double maximumDuration = (double)appStart / 1000 + (halfBuildingCorridorRow + halfBuildingCorridorColumn) / swatTeamMinSpeed + 0.1; // 0.1 to let the app stop
  uint32_t appEnd = std::ceil(maximumDuration * 1000) - 100; // applications end time in milliseconds

  std::vector< Ptr<Building> > rooms = GridBuildings::CreateGridBuildings (numRoomsRow,
                                                                           numRoomsColumn,
                                                                           corridorWidth,
                                                                           roomWidthX,
                                                                           roomWidthY,
                                                                           roomHeight);

  // drop the base stations with wired backhaul at the corners of the scenario
  PsSimulationConfig::SetConstantPositionMobility (fixedWiredBackhaulNodes.Get(0), Vector(-1, -1, bsAntennaHeight));
  PsSimulationConfig::SetConstantPositionMobility (fixedWiredBackhaulNodes.Get(1), Vector(buildingSizeX + 1, -1, bsAntennaHeight));
  PsSimulationConfig::SetConstantPositionMobility (fixedWiredBackhaulNodes.Get(2), Vector(buildingSizeX + 1, buildingSizeY + 1, bsAntennaHeight));
  PsSimulationConfig::SetConstantPositionMobility (fixedWiredBackhaulNodes.Get(3), Vector(-1, buildingSizeY + 1, bsAntennaHeight));

  // drop the SWAT teams
  PsSimulationConfig::DropSwatTeams (swatTeams, iabNodesForSwatTeams, buildingSizeX, buildingSizeY, ueAntennaHeight, randomSpeed, maximumDuration);
  BuildingsHelper::MakeMobilityModelConsistent ();

  NetDeviceContainer donorDevs = mmWaveHelper->InstallEnbDevice (fixedWiredBackhaulNodes);
  NetDeviceContainer iabDevs;
  if (useIab)
  {
    iabDevs = mmWaveHelper->InstallIabDevice (iabNodesForSwatTeams);
  }
  NetDeviceContainer ueDevs = mmWaveHelper->InstallUeDevice (allOfficers);

  // Install the IP stack on the UEs
  Ipv4InterfaceContainer ueIpIface = PsSimulationConfig::InstallUeInternet (epcHelper, allOfficers, ueDevs);

  // Initialize the channel and attach
  if (useIab)
  {
    NetDeviceContainer possibleBaseStations(iabDevs);
    mmWaveHelper->AttachIabToClosestWiredEnb (iabDevs, donorDevs);
    mmWaveHelper->AttachToClosestEnbWithDelay (ueDevs, possibleBaseStations, Seconds(0.3));
  }
  else
  {
    mmWaveHelper->AttachToClosestEnb (ueDevs, donorDevs);
  }

  // each swat member streams an uplink video feed
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> ulStream = asciiTraceHelper.CreateFileStream (filePath + "ul-app-trace.txt"); // trace file for UL traffic
  uint16_t ulPort = 1235; // port on the remote host to which the UL traffic is delivered
  for (uint32_t swIndex = 0; swIndex < allOfficers.GetN(); ++swIndex)
  {
    PsSimulationConfig::SetupUplinkUdpFlow (
      allOfficers.Get(swIndex), // source
      remoteHostPair.first, // sink
      remoteHostPair.second, // sink address
      ulPort++, // port
      MicroSeconds (packetSize * 8 / officersVideoRate * 1e6), // time interval between consecutive packets [us]
      MilliSeconds (appStart), // start time of the application [ms]
      MilliSeconds (appEnd), // end time of the application [ms]
      ulStream // trace
    );
  }

  NS_LOG_DEBUG ("UDP application interpacket interval " << packetSize * 8 / officersVideoRate * 1e6 << " micro seconds");

  mmWaveHelper->EnableTraces ();

  AsciiTraceHelper ascii;
  MobilityHelper::EnableAsciiAll (ascii.CreateFileStream ("mobility-trace-example.mob"));

  PrintHelper::PrintGnuplottableBuildingListToFile ("rooms.txt");
  PrintHelper::PrintGnuplottableNodeListToFile ("nodes.txt");

  NS_LOG_DEBUG ("Simulation stops at " << maximumDuration << " seconds");
  Simulator::Stop(Seconds (maximumDuration));
  Simulator::Run();

  return 0;
}
