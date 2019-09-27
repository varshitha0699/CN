#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("SecondscriptExample");
int main(int argc, char *argv[])
{
  bool verbose=true;
  uint32_t nCsma=2;
  CommandLine cmd;
  cmd.AddValue("nCsma","Number of\"extra\" CSMA nodes/devices",nCsma);
  cmd.AddValue("verbose"," Tell echo applications to log if true",verbose);
  cmd.Parse(argc,argv);
  if(verbose)
  {
  LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
  }
  nCsma=nCsma==0?1:nCsma;
  NodeContainer p2pNodes;
  p2pNodes.Create(2);
   NodeContainer p2pNodes1;
  p2pNodes1.Create(2);
  NodeContainer csmaNodes;
  csmaNodes.Add(p2pNodes.Get(1));
  csmaNodes.Create(nCsma);
  csmaNodes.Add(p2pNodes1.Get(0));
  PointToPointHelper pointtopoint;
  pointtopoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointtopoint.SetChannelAttribute("Delay",StringValue("2ms"));
  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
  PointToPointHelper pointtopoint1;
  pointtopoint1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointtopoint1.SetChannelAttribute("Delay",StringValue("2ms"));
  NetDeviceContainer p2pdevices;
  p2pdevices=pointtopoint.Install(p2pNodes); 
  NetDeviceContainer csmadevices;
  csmadevices =csma.Install(csmaNodes);
  
  NetDeviceContainer p2pdevices1;
  p2pdevices1=pointtopoint1.Install(p2pNodes1);
  
  InternetStackHelper stack;
  stack.Install(p2pNodes.Get(0));
    stack.Install(csmaNodes);
      stack.Install(p2pNodes1.Get(1));
      
      Ipv4AddressHelper address;
      address.SetBase("10.1.1.0","255.255.255.0");
      
      Ipv4InterfaceContainer p2pinterfaces;
      p2pinterfaces=address.Assign(p2pdevices);
      address.SetBase("10.1.2.0","255.255.255.0");
      
          Ipv4InterfaceContainer csmainterfaces;
      csmainterfaces=address.Assign(csmadevices);
      address.SetBase("10.1.3.0","255.255.255.0");
      
      Ipv4InterfaceContainer p2pinterfaces1;
      p2pinterfaces1=address.Assign(p2pdevices1);
      UdpEchoServerHelper echoserver(9);
      ApplicationContainer serverapps=echoserver.Install(csmaNodes.Get(nCsma)); 
      serverapps.Start(Seconds(1.0));
      serverapps.Stop(Seconds(10.0));
      
      UdpEchoServerHelper echoserver1(10);
      ApplicationContainer serverapps1=echoserver1.Install(csmaNodes.Get(nCsma));
      serverapps1.Start(Seconds(1.0));
      serverapps1.Stop(Seconds(10.0));
      
      UdpEchoClientHelper echoClient(csmainterfaces.GetAddress(nCsma),9);
      echoClient.SetAttribute("MaxPackets",UintegerValue(1));
      echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
      echoClient.SetAttribute("PacketSize",UintegerValue(1024));
      
      ApplicationContainer clientApps=echoClient.Install(p2pNodes.Get(0));
      clientApps.Start(Seconds(2.0));
      clientApps.Stop(Seconds(10.0));
      
      UdpEchoClientHelper echoClient1(csmainterfaces.GetAddress(nCsma),10);
      echoClient1.SetAttribute("MaxPackets",UintegerValue(2));
      echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
      echoClient1.SetAttribute("PacketSize",UintegerValue(512));
      
      ApplicationContainer clientApps1=echoClient1.Install(p2pNodes1.Get(1));
      clientApps1.Start(Seconds(2.0));
      clientApps1.Stop(Seconds(10.0));
       
       
       Ipv4GlobalRoutingHelper::PopulateRoutingTables();
       pointtopoint.EnablePcapAll("second");
       csma.EnablePcap("second",csmadevices.Get(1),true);
       pointtopoint1.EnablePcapAll("second");
       Simulator::Run();
       Simulator::Destroy();
       return 0;
}
