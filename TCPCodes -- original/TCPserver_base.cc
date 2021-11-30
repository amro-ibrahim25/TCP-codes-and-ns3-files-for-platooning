#include <iostream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TCPAcceptor.h"
#include "ns3/vector.h"
#include "ns3/string.h"
#include "ns3/socket.h"
#include "ns3/double.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/command-line.h"
#include "ns3/mobility-model.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/node-container.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
//#include "ns3/pyviz.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/error-model.h"

NS_LOG_COMPONENT_DEFINE ("WifiSimpleOcb");
using namespace ns3;

using namespace std;
NodeContainer c ;
NodeContainer d ;
double interval = 0.1; // seconds
//double interval1 = 0.01; // seconds
double CAMtime=0.1;
double cam[1200];
double DENM[1200];
int totalTCP=0;
int numVehicleNodes, numVehicleNodesCity;
int num;
//socket declaration
Ptr<Socket> source[500];
Ptr<Socket> recvSink[5000];
Ptr<Socket> sourceCity[5000];
Ptr<Socket> recvSinkCity[5000];
double distanceTraffic;
int numberOfDetailsSent=10;
uint32_t packetSize = 232; // bytes
        uint32_t numPackets = 1;
        Time interPacketInterval = Seconds (interval);	

Ptr<UniformRandomVariable> randomTimeRange = CreateObject<UniformRandomVariable>();
Ptr<UniformRandomVariable> randomTimeRangePlatoon = CreateObject<UniformRandomVariable>();
double randomTime[5000];
  //TCPStream* stream2= NULL;
  //TCPAcceptor* acceptor2=NULL;
    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;


class MyHeader : public Header 
    {
    public:
    //uint16_t* foo;
      MyHeader ();
      virtual ~MyHeader ();
    
      void SetData (uint32_t* data);
      uint32_t* GetData (void) const;
    
      static TypeId GetTypeId (void);
      virtual TypeId GetInstanceTypeId (void) const;
      virtual void Print (std::ostream &os) const;
      virtual void Serialize (Buffer::Iterator start) const;
      virtual uint32_t Deserialize (Buffer::Iterator start);
      virtual uint32_t GetSerializedSize (void) const;
     int lenght;
    private:
    uint32_t* m_data;
    };
    
MyHeader::MyHeader ()
    {
      // we must provide a public default constructor, 
      // implicit or explicit, but never private.
	m_data = new uint32_t[0];
       lenght =2;
    }
 MyHeader::~MyHeader ()
    {
	delete[] m_data;
    }
   
TypeId
    MyHeader::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::MyHeader")
        .SetParent<Header> ()
        .AddConstructor<MyHeader> ()
      ;
      return tid;
    }
    
     

TypeId
    MyHeader::GetInstanceTypeId (void) const
    {
      return GetTypeId ();
    }
    
     void
    MyHeader::Print (std::ostream &os) const
    {
      // This method is invoked by the packet printing
      // routines to print the content of my header.
      //os << "data=" << m_data << std::endl;
      for(int r=0; r<lenght; r++)
      {
      os <<  m_data[r] << std :: endl;
    }
}
 uint32_t
 MyHeader::GetSerializedSize (void) const
    {
      // we reserve 2 bytes for our header.
      return 68;
    }
void
    MyHeader::Serialize (Buffer::Iterator start) const
    {
      // we can serialize two bytes at the start of the buffer.
      // we write them in network byte order.
      for (int i=0; i<lenght; i++)
      {
      start.WriteHtonU16 (m_data[i]);
  }
    }
uint32_t
    MyHeader::Deserialize (Buffer::Iterator start)
   {
      // we can deserialize two bytes from the start of the buffer.
      // we read them in network byte order and store them
      // in host byte order.
      for(int j=0; j<lenght; j++)
      {
      m_data[j] = start.ReadNtohU16 ();
    	
      // we return the number of bytes effectively read.
  }
      return 68;
  
    }

void 
    MyHeader::SetData (uint32_t* data)
    {
     for(int i =0 ; i<lenght ; i++)
        {
	 m_data[i] = data[i];
        }
   
    }
    
   uint32_t* 
    MyHeader::GetData (void) const
    {
      
      return m_data;
    }



class MyReceiverCar
{
//char c1 = '	';
char snd[1];
float timeTaken, differenceTime;
int lengt;
Ptr<Packet> dummyPacket;
Ptr <Packet> p;
Address from;
uint32_t* x;
char tabsnd1[4];
public:
void InformMatlab (Ptr<Socket> socket){
timeTaken=float((long int)(Simulator::Now().GetSeconds()*1000))/1000;
p=socket->RecvFrom (from);
MyHeader header;
static int total_received[5][5];
static float delay_array[5][5];//[0][1]means0->1
p->RemoveHeader (header);
x = header.GetData ();


if((long int)((float(x[1])/100)*10) == (long int)(Simulator::Now().GetSeconds()*10))//send only if the information belongs to the present step. It disregards older messages
{
timeTaken=abs(ceil(1000*(timeTaken-float(x[1])/100)));//in milliseconds with no precision
if(x[0]<socket->GetNode()->GetId()){//send to matlab only if receivernode is a follower and not a predecessor
	
	  std::cout <<"At " <<Simulator::Now().GetSeconds() << ", Vehicle: " << socket->GetNode()->GetId() << " received packet from Vehicle: " << x[0] <<" ("<< InetSocketAddress::ConvertFrom (from).GetIpv4 ()<<")"<< " with a delay of "<<timeTaken<<"ms" <<"\n" ;	
	delay_array[x[0]][socket->GetNode()->GetId()]=delay_array[x[0]][socket->GetNode()->GetId()]+timeTaken;
	total_received[x[0]][socket->GetNode()->GetId()]=total_received[x[0]][socket->GetNode()->GetId()]+1;
	cout<<" - average delay for this link is: " << delay_array[x[0]][socket->GetNode()->GetId()]/total_received[x[0]][socket->GetNode()->GetId()] << "ms" << "\n";
	std::stringstream toMATLAB1,toMATLAB2,toMATLAB3,toMATLAB4;
	toMATLAB1 << socket->GetNode()->GetId(); toMATLAB2 << x[0]; 

	std::string stringtoMATLAB1, stringtoMATLAB2, stringtoMATLAB3, stringtoMATLAB4;
	stringtoMATLAB1=toMATLAB1.str(); stringtoMATLAB2=toMATLAB2.str(); 
		if (timeTaken<10){
		toMATLAB3 << 0;
		toMATLAB4 << timeTaken;
		}
		else{
		toMATLAB3 << timeTaken;
		}
		stringtoMATLAB3=toMATLAB3.str(); stringtoMATLAB4=toMATLAB4.str();
		std::string CombineString;
		if (timeTaken<10){
		CombineString = stringtoMATLAB1+ stringtoMATLAB2+ stringtoMATLAB3+ stringtoMATLAB4;
		}
		else{
		CombineString = stringtoMATLAB1+ stringtoMATLAB2+ stringtoMATLAB3;
		}

std::stringstream snd1;snd1 << std::to_string(timeTaken);
//snd1 << Simulator::Now().GetSeconds();
std::string newstringsnd1;
newstringsnd1=snd1.str();
strncpy(tabsnd1, CombineString.c_str(), sizeof(tabsnd1));

	lengt=sizeof(tabsnd1);///////////////////////////////////////////////////////////////////////////////////////////////////////
	stream->send(tabsnd1,lengt);/////////////////////////////////////////////////////////////////////////////////////////
	//differenceTime=((float(int(Simulator::Now().GetSeconds()*10))/10)+0.1)-Simulator::Now().GetSeconds();
	//Simulator::Schedule(Seconds(differenceTime), &sendMatlabnow, tabsnd1);
numberOfDetailsSent++;
}
}
}
};
MyReceiverCar MyReceiverCarInfo;

void sendMatlabnow(){//sending dummies to matlab in case packet is lost
char tabsnd[4];
std::stringstream toMATLAB;toMATLAB << 0;
std::string stringtoMATLAB;stringtoMATLAB=toMATLAB.str();
std::string CombineStrn;CombineStrn = stringtoMATLAB + stringtoMATLAB + stringtoMATLAB + stringtoMATLAB;
strncpy(tabsnd, CombineStrn.c_str(), sizeof(tabsnd));
for (int q=0; q<(((5-1)*5/2)-numberOfDetailsSent); q++){/////////////////////////////////////////////////////////////////////////
	stream->send(tabsnd,sizeof(tabsnd));	
}

}

void showPosition(Ptr<Node> node, double deltaTime)
{
  uint32_t nodeId = node->GetId();
  Ptr<ConstantVelocityMobilityModel> mobModel = node->GetObject<ConstantVelocityMobilityModel>();
  Vector pos = mobModel->GetPosition();
  Vector vel = mobModel->GetVelocity();
std:: ofstream myfile1("PositionAndVelocity.txt", std::ios_base::app);
myfile1  << "At " << Simulator::Now().GetSeconds() << " sec  node " << nodeId
       << ": Position(" << pos.x << ", " << pos.y << ", " << pos.z
       << ")" << ": Velocity(" << vel.x << ", " << vel.y << ", " << vel.z
       << ")" << "\n";

std::cout  << "At " << Simulator::Now().GetSeconds() << " sec  node " << nodeId
       << ": Position(" << pos.x << ", " << pos.y << ", " << pos.z
       << ")" << ": Velocity(" << vel.x << ", " << vel.y << ", " << vel.z
       << ")" << std::endl;

  //Simulator::Schedule(Seconds(deltaTime), &showPosition, node,deltaTime);
myfile1.close();
}
/*
void
 CourseChange(std::string context, Ptr<const MobilityModel>model)
{
Vector position = model->GetPosition ();
Vector vel = model->GetVelocity();
  Ptr<Node> n = model->GetObject<Node>();
  NS_LOG_UNCOND (" At " << Simulator::Now().GetSeconds() << " sec " << "->" << " node " << n->GetId() << " " << "->" << " "  <<     "Position x = " << position.x << " " << ",  y = " << position.y << " " << "->"  << " velocity  x= " << vel.x << " " << ", y= " << vel.y);

// Simulator::Schedule(Seconds(0.0), &showPosition, c.Get(0), interval);
       // Simulator::Schedule(Seconds(0.0), &showPosition, c.Get(1), interval);
        //Simulator::Schedule(Seconds(0.0), &showPosition, c.Get(2), interval);
}
*/
void SetPosition(Ptr<Node> node, Vector pos)
{
Ptr<ConstantVelocityMobilityModel> mobility= node->GetObject<ConstantVelocityMobilityModel>();
mobility->SetPosition(pos);
}

void SetVelocity (Ptr<Node> node, Vector vel)
{
  Ptr<ConstantVelocityMobilityModel> mobility1 = node->GetObject<ConstantVelocityMobilityModel> ();
  mobility1->SetVelocity (vel);

}
char c1 = '	';
char snd[2];
int lengt;



static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval )
	{
	  if (pktCount > 0)
	    {
	     // socket->Send (Create<Packet> (pktSize));
		//Ptr<Packet> p = Create<Packet> (pktSize);
		MyHeader header;
		uint32_t Data[2]; 
		//uint8_t *buffer = (uint8_t*)Data;
		//uint32_t size = 2;
		Ptr<Packet> p = Create<Packet> (pktSize);//(buffer, size);
		Data[0] = socket->GetNode()->GetId();
		Data[1] = Simulator::Now().GetSeconds()*100;
		header.SetData (Data);
		p->AddHeader (header);
		socket->Send (p);
     //Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize,pktCount - 1, pktInterval);
		//NS_LOG_UNCOND ("One CAM Packet Sent!");
		//std::cout <<"At " <<Simulator::Now().GetSeconds() << " by Vehicle: " << socket->GetNode()->GetId() <<"\n" ;
	    }
	  else
	    {
	      socket->Close ();
	    }
	}


static void Pause()
{
//Create the TCP stream class and acceptor class object


        size_t length;
        char buffer[614400];
   
        printf("\nPausing the running simulation\n");
       //The code pauses at the "receive()" function)
if(numberOfDetailsSent<((5-1)*5/2)){
	sendMatlabnow();
}
numberOfDetailsSent=0;

       if (stream != NULL)           //works for time = anytime but t = 0.1
            { 
                if((length=stream->receive(buffer, sizeof(buffer))) >0)
                {
             //Ideally this is where the NS3 mobility model update code will go!!!!
                buffer[length]='\0';
                     //printf("%s\n",buffer);
              //Parse the data from the matlab to update the mobility model

double pos[60000];						/////////////////////////////
int u=0;
char *tempA;
tempA=strtok(buffer,",");

                        
while (tempA != NULL)
  {
    pos[u] = atof(tempA);
    tempA=strtok(NULL,",");
    u=u + 1;
  }
/////////////////////////////////////
if(u<totalTCP){
length = stream->receive(buffer, sizeof(buffer));
u=u-1;
buffer[length]='\0';
tempA=strtok(buffer,",");
while (tempA != NULL)
  {
    pos[u] = atof(tempA);
    tempA=strtok(NULL,",");
    u=u + 1;
  }
}
//redundant
//////////////////////////////////////


for(num=0;num<numVehicleNodes;num++){

Simulator::ScheduleNow(&SetPosition,c.Get(num), Vector(pos[num*2],pos[1+num*2],0.0));

Simulator::ScheduleNow(&SetVelocity,c.Get(num), Vector(pos[numVehicleNodes*2+num*2],pos[numVehicleNodes*2+num*2+1],0));
}

/////////////////////////////////mobility city//////////////////////////////////////////
//lane0
for(num=0;num<numVehicleNodesCity/2;num++){
if(num<numVehicleNodesCity/4)
Simulator::ScheduleNow(&SetPosition,d.Get(num), Vector(pos[0]+distanceTraffic*(num+1),pos[1]+3.3,0.0));
else
Simulator::ScheduleNow(&SetPosition,d.Get(num), Vector(pos[0]-distanceTraffic*(numVehicleNodesCity-(num+1)),pos[1]+3.3,0.0));
Simulator::ScheduleNow(&SetVelocity,d.Get(num), Vector(pos[numVehicleNodes*2],pos[numVehicleNodes*2+1],0));
}
//lane2
int k=numVehicleNodesCity/2;
for(num=0;num<numVehicleNodesCity/2;num++){
if(num<numVehicleNodesCity/4)
Simulator::ScheduleNow(&SetPosition,d.Get(k), Vector(pos[0]+distanceTraffic*(num+1),pos[1]-3.3,0.0));
else
Simulator::ScheduleNow(&SetPosition,d.Get(k), Vector(pos[0]-distanceTraffic*(numVehicleNodesCity-(num+1)),pos[1]-3.3,0.0));
Simulator::ScheduleNow(&SetVelocity,d.Get(k), Vector(pos[numVehicleNodes*2],pos[numVehicleNodes*2+1],0));
k++;
}
///////////////////////////////////////////////////////////////////////////////////////

/*
double v0 = pos[6] - (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v1 = pos[8] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v2 = pos[10] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v3 = pos[12] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v4 = pos[28] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v5 = pos[30] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v6 = pos[32] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v7 = pos[34] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v8 = pos[36] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));
double v9 = pos[38] + (1 + ( std::rand() % ( 5 - 1 + 1 ) ));

 

std::stringstream ss;
ss << v0;
std::string newstring;
newstring=ss.str();
      // std::cout << newstring;

std::stringstream ss1;
ss1 << v1;
std::string newstring1;
newstring1=ss1.str();
       //std::cout << newstring1;

std::stringstream ss2;
ss2 << v2;
std::string newstring2;
newstring2=ss2.str();
      // std::cout << newstring2;

std::stringstream ss3;
ss3 << v3;
std::string newstring3;
newstring3=ss3.str();

std::stringstream ss4;
ss4 << v4;
std::string newstring4;
newstring4=ss4.str();

std::stringstream ss5;
ss5 << v5;
std::string newstring5;
newstring5=ss5.str();

std::stringstream ss6;
ss6 << v6;
std::string newstring6;
newstring6=ss6.str();

std::stringstream ss7;
ss7 << v7;
std::string newstring7;
newstring7=ss7.str();

std::stringstream ss8;
ss8 << v8;
std::string newstring8;
newstring8=ss8.str();

std::stringstream ss9;
ss9 << v9;
std::string newstring9;
newstring9=ss9.str();
std::cout << "\n";

std::string Combine;

Combine=newstring + ','+newstring1+','+newstring2+','+newstring3+','+newstring4+','+newstring5+','+newstring6+','+newstring7+','+newstring8+','+newstring9;

Combine=newstring+ ','+newstring1+ ','+newstring2+ ','+newstring3;

//std::cout << Combine;
//std::cout << "\n";
					//int len1 = Combine.length();

//char msg2[]="11,12,14,15,16,1";

char tab2[30];///////////////
strncpy(tab2, Combine.c_str(), sizeof(tab2));
tab2[sizeof(tab2) - 1] = 0;


                     //char msg2[]="Position Updated";
                     //std::cout << "\n";

//                     stream2->send(tab2,len1);
 
                    buffer[0]='\0';
*/
// CAM and DENM message generation condition based on the position difference w.r.t to one vehicle


std::cout<<"CAM has been generated at " <<Simulator::Now().GetSeconds();
std::cout << "\n";
///////////////////////////////////congestion transmission///////////////////////////////////
for(num=0;num<numVehicleNodesCity;num++){
if((sourceCity[num]->GetNode ()->GetId ())==unsigned(numVehicleNodes))
	Simulator::ScheduleWithContext (sourceCity[num]->GetNode ()->GetId (),
                                Seconds (0.0), &GenerateTraffic,
                                 sourceCity[num], packetSize, numPackets, interPacketInterval);////randomTime[num+numVehicleNodes]
else
	Simulator::ScheduleWithContext (sourceCity[num]->GetNode ()->GetId (),
                                Seconds (randomTime[num+numVehicleNodes]), &GenerateTraffic,
                                 sourceCity[num], packetSize, numPackets, interPacketInterval);
}
//Simulator::ScheduleWithContext (sourceCity[numVehicleNodesCity-1]->GetNode ()->GetId (),
  //                              Seconds (0.99), &GenerateTraffic,
    //                             sourceCity[num], packetSize, numPackets, interPacketInterval);
////////////////////////////////////////////////////////////////////////////////////////////

for(num=0;num<numVehicleNodes;num++){
if(source[num]->GetNode ()->GetId ()==0)
Simulator::ScheduleWithContext (source[num]->GetNode ()->GetId (),			
                                Seconds (randomTime[num]), &GenerateTraffic,
                                 source[num], packetSize, numPackets, interPacketInterval);
else
Simulator::ScheduleWithContext (source[num]->GetNode ()->GetId (),			
                                Seconds (randomTime[num]), &GenerateTraffic,
                                 source[num], packetSize, numPackets, interPacketInterval);
}


                        }


    }


for(num=0;num<numVehicleNodes;num++){
Simulator::Schedule(Seconds(0.0), &showPosition, c.Get(num), interval);
}
Simulator::Schedule(Seconds(0.1), Pause);
//std::cout << "\n";
}




int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 4) {
        printf("usage: server <port> [<ip>]\n");
        exit(1);
    }

    if (argc == 3) {
        acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }
    else {
        acceptor = new TCPAcceptor(atoi(argv[1]));
    }
    if (acceptor->start() == 0) {
        while (1) {
            stream = acceptor->accept();
            if (stream != NULL) {
                size_t len;
		
                char line[61440];
                if ((len = stream->receive(line, sizeof(line))) > 0) {
			line[len]='\0';



double pos1[60000];
int u=0;
char *tempA1;
tempA1=strtok(line,",");

                        
while (tempA1 != NULL)
  {
    pos1[u] = atof(tempA1);
    tempA1=strtok(NULL,",");
    u=u + 1;
  }
totalTCP=5*pos1[0];//5 bytes are the details of each vehicle
/////////////////////////////
if(u<totalTCP){
len = stream->receive(line, sizeof(line));
u=u-1;
line[len]='\0';
tempA1=strtok(line,",");
while (tempA1 != NULL)
  {
    pos1[u] = atof(tempA1);
    tempA1=strtok(NULL,",");
    u=u + 1;
  }
}
//redundant because it does not receive long buffer in 1 transaction
///////////////////////////////////

numVehicleNodes=pos1[0];
printf("Number of nodes are %d\n", numVehicleNodes);
 
std::string phyMode ("OfdmRate6MbpsBW10MHz");

bool verbose = false;

CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.Parse (argc, argv);
  // Convert to time object

  c.Create (numVehicleNodes);
distanceTraffic = 1000;
numVehicleNodesCity=0;//multiple of 4
  // The below set of helpers will help us to put together the wifi NICs we want
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  //The tx start and end with levels, the value is in dbm
  wifiPhy.Set ("TxPowerStart", DoubleValue (24) );
  wifiPhy.Set ("TxPowerEnd",   DoubleValue (24) );
  wifiPhy.Set ("TxPowerLevels", UintegerValue(1) );
  YansWifiChannelHelper wifiChannel;
  //wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel"); 
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::ThreeLogDistancePropagationLossModel","Exponent0",DoubleValue (1.9),"Distance0",DoubleValue (10),"Exponent1",DoubleValue (3.8),"Distance1",DoubleValue (80), "ReferenceLoss",DoubleValue (61.8));
  wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel","m0", DoubleValue(3), "m1", DoubleValue(1.5), "m2", DoubleValue(1),"Distance1",DoubleValue (50),"Distance2",DoubleValue (150)); 

  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);
  // ns-3 supports generate a pcap trace
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
///////////////////////////city traffic////////////////////////////////////////////////////////////////////
  d.Create (numVehicleNodesCity); 
  YansWifiPhyHelper wifiPhyCity =  YansWifiPhyHelper::Default ();
  //The tx start and end with levels, the value is in dbm
  wifiPhyCity.Set ("TxPowerStart", DoubleValue (24) );
  wifiPhyCity.Set ("TxPowerEnd",   DoubleValue (24) );
  wifiPhyCity.Set ("TxPowerLevels", UintegerValue(1) );
  YansWifiChannelHelper wifiChannelCity;
  wifiChannelCity.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannelCity.AddPropagationLoss("ns3::ThreeLogDistancePropagationLossModel","Exponent0",DoubleValue (1.9),"Distance0",DoubleValue (10),"Exponent1",DoubleValue (3.8),"Distance1",DoubleValue (80), "ReferenceLoss",DoubleValue (61.8));
  wifiChannelCity.AddPropagationLoss("ns3::NakagamiPropagationLossModel","m0", DoubleValue(3), "m1", DoubleValue(1.5), "m2", DoubleValue(1),"Distance1",DoubleValue (50),"Distance2",DoubleValue (150)); 
 // Ptr<YansWifiChannel> channelCity = wifiChannelCity.Create ();
  wifiPhyCity.SetChannel (channel);
  // ns-3 supports generate a pcap trace
  wifiPhyCity.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  NqosWaveMacHelper wifi80211pMacCity = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi80211pCity = Wifi80211pHelper::Default ();
  wifi80211pCity.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode),"NonUnicastMode" , StringValue (phyMode));
 NetDeviceContainer devicesCity = wifi80211pCity.Install (wifiPhyCity, wifi80211pMacCity, d);
//////////////////////////////////////////////////////////////////////////////

  if (verbose)
    {
      wifi80211p.EnableLogComponents ();      // Turn on all Wifi 802.11p logging
    }

  wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode),"NonUnicastMode" , StringValue (phyMode));
 NetDeviceContainer devices = wifi80211p.Install (wifiPhy, wifi80211pMac, c);
 Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/DcaTxop/Queue/MaxDelay",TimeValue(Seconds(0.1)));
 Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/CcaMode1Threshold",DoubleValue (-85.0));/////////////
 Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::YansWifiPhy/EnergyDetectionThreshold",DoubleValue (-82.0));////////
AsciiTraceHelper ascii;
//wifiPhy.EnableAsciiAll (ascii.CreateFileStream("Interface.tr"));

MobilityHelper mobility;
int k=0;
for (int i=0; i<numVehicleNodes; i++)
{
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (pos1[2*i+1], pos1[2*i+2], 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (c.Get(k));
Ptr <ConstantVelocityMobilityModel> model = c.Get (k) -> GetObject <ConstantVelocityMobilityModel> ();
model-> SetVelocity (Vector (pos1[numVehicleNodes*2+i*2+1],pos1[numVehicleNodes*2+i*2+2],0));
k++;
}
////////////////////////mobility city///////////////////////////////////////
MobilityHelper mobilityCity;
//lane0
for (int i=0; i<numVehicleNodesCity/2; i++)
{
  Ptr<ListPositionAllocator> positionAllocCity = CreateObject<ListPositionAllocator> ();
  if(i<numVehicleNodesCity/4)
  positionAllocCity->Add (Vector ((pos1[1]+distanceTraffic*(i+1)), pos1[2]+3.3, 0.0));
  else
  positionAllocCity->Add (Vector ((pos1[1]-distanceTraffic*(numVehicleNodesCity/2-(i+1))), pos1[2]+3.3, 0.0));  
  mobilityCity.SetPositionAllocator (positionAllocCity);
  mobilityCity.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobilityCity.Install (d.Get(i));
  Ptr <ConstantVelocityMobilityModel> modelCity = d.Get (i) -> GetObject <ConstantVelocityMobilityModel> ();
  modelCity-> SetVelocity (Vector (pos1[numVehicleNodes*2+1],pos1[numVehicleNodes*2+2],0));
}
//lane2
k=numVehicleNodesCity/2;
for (int i=0; i<numVehicleNodesCity/2; i++)
{
  Ptr<ListPositionAllocator> positionAllocCity = CreateObject<ListPositionAllocator> ();
  if(i<numVehicleNodesCity/4)
  positionAllocCity->Add (Vector ((pos1[1]+distanceTraffic*(i+1)), pos1[2]-3.3, 0.0));
  else
  positionAllocCity->Add (Vector ((pos1[1]-distanceTraffic*(numVehicleNodesCity/2-(i+1))), pos1[2]-3.3, 0.0));  
  mobilityCity.SetPositionAllocator (positionAllocCity);
  mobilityCity.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobilityCity.Install (d.Get(k));
  Ptr <ConstantVelocityMobilityModel> modelCity = d.Get (k) -> GetObject <ConstantVelocityMobilityModel> ();
  modelCity-> SetVelocity (Vector (pos1[numVehicleNodes*2+1],pos1[numVehicleNodes*2+2],0));
  k++;
}
//////////////////////////////////////////////////////////////////////////////

for(num=0;num<numVehicleNodes;num++){
Simulator::Schedule(Seconds(0.0), &showPosition, c.Get(num), 0);
}

  InternetStackHelper internet;
  internet.Install (c);
 
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

/////////////////////city internet/////////////////////////////
  //InternetStackHelper internetCity;
  internet.Install (d);
 
  //Ipv4AddressHelper ipv4City;
  //ipv4City.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iCity = ipv4.Assign (devicesCity);
///////////////////////////////////////////////////////////////

MobilityHelper::EnableAsciiAll(ascii.CreateFileStream("mobility-trace-example.mob"));
Config::SetDefault ("ns3::PacketSocket::RcvBufSize", StringValue ("33554432"));
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);

for(num=0;num<numVehicleNodes;num++){
  recvSink[num] = Socket::CreateSocket (c.Get (num), tid);
  recvSink[num]->Bind (local);
  if (num<5)
  recvSink[num]->SetRecvCallback (MakeCallback (&MyReceiverCar::InformMatlab, &MyReceiverCarInfo));
}

for(num=0;num<numVehicleNodes;num++){
  source[num] = Socket::CreateSocket (c.Get (num), tid);
  source[num]->SetAllowBroadcast (true);
  source[num]->Connect (remote);
}

///////////////////////////sockets////////////////////////////////////

  InetSocketAddress remoteCity = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);//////////////////////////////////////////////
  InetSocketAddress localCity = InetSocketAddress (Ipv4Address::GetAny (), 80);

for(num=0;num<numVehicleNodesCity;num++){
  recvSinkCity[num] = Socket::CreateSocket (d.Get (num), tid);
  recvSinkCity[num]->Bind (localCity);
}

for(num=0;num<numVehicleNodesCity;num++){
  sourceCity[num] = Socket::CreateSocket (d.Get (num), tid);
  sourceCity[num]->SetAllowBroadcast (true);
  sourceCity[num]->Connect (remoteCity);
}


for(int m=0; m<numVehicleNodes+numVehicleNodesCity;m++){
randomTimeRange->SetAttribute( "Min", DoubleValue( 0.0 ) );
randomTimeRange->SetAttribute( "Max", DoubleValue( 0.1 ) );
//randomTime[m]=float(int(randomTimeRange->GetValue()*1000))/1000;
randomTime[m]=randomTimeRange->GetValue();
if (m<5){
  randomTimeRangePlatoon->SetAttribute( "Min", DoubleValue( 0.0 ) );
  randomTimeRangePlatoon->SetAttribute( "Max", DoubleValue( 0.1 ) );	
  randomTime[m]=randomTimeRangePlatoon->GetValue();
}
//std::cout<<randomTime[m]<<"\n";
}


Simulator::Schedule(Seconds(0.1), Pause);

  //AnimationInterface anim("Interface.xml");

  Simulator::Stop(Seconds(180.0));
  Simulator::Run ();

}
      

}

}


}
  Simulator::Destroy ();
  delete stream;
  return 0;

    perror("Could not start the server");
    exit(-1);
}
