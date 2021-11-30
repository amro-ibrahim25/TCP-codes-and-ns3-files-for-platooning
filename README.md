# TCP codes -- ns3 files
 Contains the ns3 files used for platooning applications.
 
For more information on how to initiate the connection between Matlab, SUMO, and ns3, please read the details in: https://www.es.ele.tue.nl/~dgoswami/crets 

---
layout: page
title: CReTS
permalink: /crets
---

# CReTS framework

 CReTS (ContRol, nEtwork and Traffic Simulator) is a  framework composed of network simulator ns-3, traffic simulator SUMO and Matlab.  ns-3 provides packet 
 reception ratios and delays to the controller implemented in Matlab. Different network congestion levels can be simulated
 by adjusting the number of communicating vehicles using SUMO and the corresponding control performance is evaluated.

<p align="center">
  <img src="{{ site.urlbase }}images/MATLAB_SUMO_ns3.png" width="800">
</p>

## Installation
### **1. Traci4Matlab**
Traci4Maltab is an Application Programming Interface (API) developed in Matlab that allows the communication between SUMO (Simulation of Urban Mobility) and an application 
developed in Matlab. Traci4Matlab implements the Traci (Traffic control interface) which is built over the TCP/IP stack, so the application is developed in Matlab 
where Matlab acts as a client and SUMO as server. Traci4Matlab can access and modify any variable from SUMO (server). 
It also helps to control SUMO objects such as the traffic lights, vehicles, road intersection, etc.

#### Traci4Matlab Installation 
Requirements: 
*  Windows 7X64 or higher
*  Matlab R2017a or higher
*  Sumo 0.19.0 or higher
   


#### Dowload
Traci4Matlab can be downloaded from Mathworks website from the following [link](https://nl.mathworks.com/matlabcentral/fileexchange/44805-traci4matlab).
To have SUMO working with Traci4Matlab properly, download the version of 2017. It can be downloaded from [here]({{ site.urlbase }}files/traci4matlab.zip).



#### Installation steps
After the installation of SUMO on windows, follow the next steps: 
*  Step 1: Creation of SUMO HOME environment Variable: 
Set up the SUMO environment variable with the value corresponding to SUMO installation root directory. 
If SUMO version is SUMO-0.23.0 and it is installed in C:\ then SUMO_HOME environment variable will be C:\SUMO-0.23.0. To do this on windows:
Right click on Windows start button &rarr Right Click on ‘Computer’ --> Click on Properties Options as shown in figure 1 --> It opens the new windows as in Figure 2 -->
Click on the Advanced 
System setting on the left --> This opens a new windows as shown in Figure 3 --> Click on the Environment Variable. --> In the system variable, click on the “New” and confgure the 
variable according to SUMO installation &rarr It opens new window, fill the details as shown in figure 4 --> Now, Look into the same field for “path” variable and add the path of 
bin directory of SUMO as mentioned in figure 5. Don’t forgot to miss the semicolon as mentioned in figure 6. If every things went well, creation of SUMO HOME environment 
Variable is done. Follow the sequence of screenshot of image.  



<figure align="center">
  <img src="{{ site.urlbase }}images/crets_1.bmp" width="600">
  <figcaption>Figure 1 Access to System Properties</figcaption>
</figure>

  
<figure align="center">
  <img src="{{ site.urlbase }}images/crets_2.bmp" width="200">
  <figcaption>Figure 2 Advanced System Properties  </figcaption>
</figure>  

 
<figure align="center">
  <img src="{{ site.urlbase }}images/crets_3.bmp" width="600">
  <figcaption>Figure 3 Environment Variable creation </figcaption>
</figure>  

<figure align="center">
  <img src="{{ site.urlbase }}images/crets_4.bmp" width="800">
  <figcaption>Figure 4 Create New environment Variable Path</figcaption>
</figure>  


<figure align="center">
  <img src="{{ site.urlbase }}images/crets_5.bmp" width="800">
  <figcaption>Figure 5 Edit Path to add SUMO bin directory path</figcaption>
</figure>  


 
*  Step 2: Set path in Matlab for Traci4Matlab: 
After download Traci4Matlab and unzip the file and save it over the Matlab Path. Normally it is found in Documents\MATLAB. 
It can be easily verified from the system where Matlab is installed. To set the path of traci4Matlab follow the steps mentioned below-
<figure align="center">
  <img src="{{ site.urlbase }}images/crets_6.bmp" width="1000">
  <figcaption>Figure 6 Setting in Matlab</figcaption>
</figure> 

Open Matlab --> In the HOME section, search for the option “Set Path” --> Select the button “Add with Subfolders” and select Traci4Matlab folder(unzip one) and finally click 
save and close.
Now after this, setting of Matlab for Traci4Matlab has been done and we have to run the file to set up the connection. Connection set up details is discussed in later 
portion of this documents.

### **2. ns-3**
**Create a Virtual Machine**
1.  First step is to install [Oracle VM Virtual Box](https://www.virtualbox.org/).
2.  Download the [Ubuntu 14.04](http://releases.ubuntu.com/14.04/) software (32 or 64 bit depending on your PC).
3.  Make a Virtual Machine with;
    - Sufficient RAM, ±10 GB
    - A hard drive space around 40 GB.
4.  Go into the settings of the machine and increase the video memory up to around 100 MB and assign more processor cores for improving the performance of Ubuntu.
5.  Leave the network settings as default, this will ensure your internet connection will be available within Ubuntu and enables the installing of git and ns-3.
6.  Choose the downloaded Ubuntu .iso file as the start-up disc and launch the Virtual Machine. Make sure Virtualization Technology is enabled in the BIOS settings of your PC.

**Prepare installation of ns-3**
Since the default installations of gcc and g++ on Ubuntu are outdated, this will result in an error for installing and building the ns-3 software.
Therefore, we install the compilers gcc and g++ version 5 (or higher).
1.  Open the terminal in Ubuntu and give the following commands to add the repository for the packages of the compilers:  
    `$ sudo add-apt-repository ppa:ubuntu-toolchain-r/test`  
    `$ sudo apt-get update`  
    `$ sudo apt-get install gcc-5 g++-5`
2.  To make the newly installed versions the default compilers, give the commands:  
    `$ sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 90 --slave /usr/bin/g++ g++ /usr/bin/g++-5`
3.  Install Git to clone the ns-3 files:  
    `$ sudo apt-get update`  
    `$ sudo apt-get install git`  

The system is now ready to install ns-3.

**Installation of ns-3**
Continue in the terminal of Ubuntu to install ns-3.
1.  Make a folder to install ns-3, e.g. "repos", and navigate to that folder:  
    `$ cd`  
    `$ mkdir repos`  
    `$ cd repos`
2.  Clone the ns-3 installation files:  
    `$ git clone https://gitlab.com/nsnam/ns-3-allinone.git`
3.  Move into the ns-3-allinone directory and download ns-3, version 3.26:  
    `$ cd ns-3-allinone`
    `$ ./download.py -n ns-3.26`
4.  The system is now ready to build the ns-3 distribution:  
    `$ ./build.py`
5.  Once the project is built, continue with the configuration of Waf, inside the ns-3.26 directory:  
    `$ cd ns-3.26`  
    `$ CXXFLAGS="-O3" ./waf configure`  
6.  Lastly, enable examples and tests to test and and verify the installation:  
    `$ ./waf configure --enable-examples --enable tests`  
    `$ ./waf build`  
    `$ ./test.py`  

The last command should ultimately produce output like:  
```Linux
...
PASS: TestSuite histogram
PASS: TestSuite ns3-wifi-interference
PASS: TestSuite ns3-tcp-cwnd
PASS: TestSuite ns3-tcp-interoperability
PASS: TestSuite sample
...
```
Extended installation steps can be found [here](https://www.nsnam.org/wiki/Installation).

### **3. MATLAB-ns3 Interface**
**Virtual Network Creation between Windows and a guest OS (Ubuntu 14.04) running on Virtual Machine.**
1.  For making two way connection between windows and a guest OS (Ubuntu 14.04), settings have to be made in both OS and in Virtual Box.
2.  Settings in Windows:
    - Go to the windows firewall section, through the control panel.
    - Click on advanced settings. In the page that pops up,
    - Click on inbound rules (left column), then on new rule (right column). Choose custom rule, set the rule to allow all programs, and any protocol. For the scope, add in the first box (local IP addresses) 192.168.56.1, and in the second box (remote IP) 192.168.56.101. Click next, select allow the connection, next, check all profiles, next, give it a name and save.
    These settings will assign a static IP to windows for TCP communication and will instruct the firewall to not block any data arriving from the afore-mentioned remote IP.
3.  Settings in Virtual Box:  
    - File -> Preferences -> Network -> Host-Only Networks -> Edit Host-Only Network -> pv4 address: 192.168.56.1, Ipv4 Network Mask: 255.255.255.0 -> Uncheck “Enable Server” option under the DHCP tab -> OK
4.  Setting in Ubuntu:  
    - Devices Tab -> Network -> Network Settings
    - Under the Adapter 1 tab, choose the options as shown in the following figure:
<figure align="center">  
  <img src="{{ site.urlbase }}images/crets_7.bmp" width="800">
  <figcaption>Figure 7 Setting in Virtual Box </figcaption>
</figure> 

 Also, changes will have to be made in the **/etc/network/interfaces** file. First of all, a static IP will have to be assigned to Ubuntu for the connection. 
 Open up the below mentioned file using the following command
```Linux
sudoedit /etc/network/interfaces
```
Edit the file so that it contains the following information:
<figure align="center">  
  <img src="{{ site.urlbase }}images/crets_8.bmp" width="800">
</figure> 


Therefore, we are assigning the static IP: 192.168.56.101 to the virtual machine OS. Also, navigate to the **/etc/network** folder and open up the **NetworkManager.conf** file. 
It should look like this:
<figure align="center">  
  <img src="{{ site.urlbase }}images/crets_9.bmp" width="800">
</figure> 

Make sure that the **[ifupdown]** managed flag is set to **false** as shown above.
Once all this is done, run the **ifconfig** command in Ubuntu terminal to see if the new static IP is listed or not. Also, try to ping one OS from the other to ensure that a 
connection has been made.

To summarize, we have created a Host only network to connect the host machine with the virtual operating system. Host-only networking provides a network 
connection between the virtual machine and the host computer, using a virtual Ethernet adapter that is visible to the host operating system. This approach 
can be useful if you need to set up an isolated virtual network. If you use host-only networking, your virtual machine and the host virtual adapter are connected 
to private Ethernet network.

### **4. Steps to integrate TCP/IP sockets API with NS3 code directory**
In order to make use of the Sockets API for establishing communication between the Matlab and NS3, the API libraries need to be inserted into the NS3 
source code directory, where NS3 will play the role of a TCP server. In order to proceed, the socket function have first been arranged into appropriate classes. 
The TCPAcceptor class is responsible for creating a socket on the server, binding a local address to the socket, listening for incoming connection and accepting 
any connection from Matlab. The TCPStream class is responsible for exchange of data between the two simulators, once a connection is established. In order to use 
these class, place the following files in the **./ns3/ns-allinone-3.23/ns-3.23/src/core/model directory**.
1. TCPStream.h
2. TCPStream.cc
3. TCPAcceptor.h
4. TCPAcceptor.cc


The aformentioned files can be downloaded from [here]({{ site.urlbase }}files/TCPCodes.zip). 

The “src” directory is the one that contains all the source code for NS3. After this step, the source filenames in the above list will have to added to 
the list of sources files in the **src/core** directory **“wscript”** file. There is no need to add header files. See the below mentioned screenshot of wscript file. 
Add in the same way as mentioned.
<figure align="center">  
  <img src="{{ site.urlbase }}images/crets_10.bmp" width="800">
</figure> 

Also, create a new folder in the **./ns3/ns-allinone-3.23/ns-3.23/scratch** directory named as **TCPCodes**, and place the above files in the folder along with 
the **“TCPserver-base.cc”** file which is the main script. Once this is done, build the NS3 modules using **“./waf build”** command to complete the inclusion of the 
new codes in the /src/core/model directory.
Once this is done, it is possible to use sockets API in NS3 and connection can be established.



### **5. SUMO-MATLAB-NS3 Interface Simulation Steps**
Once all the pre-setting has been done, now to run the complete interface of SUMO-Matlab-NS3, few setting needs to be done in SUMO and Matlab. 
Here we will run **Hello.sumocfg** as an example. For different application, changes have to be done accordingly.

**SUMO settings** 
Navigate to **C:\sumo-0.23.0\docs\tutorial\hello\data\** and open **hello.sumocfg** file. Change the file as mentioned below.

`<?xml version="1.0" encoding="UTF-8"?>`
`<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd">`
`<input>`
`<net-file value="hello.net.xml"/>`
`<route-files value="hello.rou.xml"/>`
`</input>`
`<time>`
`<begin value="0"/>`
`<end value="60"/>`
`<step-length value="0.1"/>`
`</time>`
`<gui_only>`
`<gui-settings-file value="hello.settings.xml"/>`
`</gui_only>`
`<traci_server>`
`<remote-port value ="8873"/>`
`</traci_server>`
`</configuration> `


 
 The last part in the previous XML code (i.e. `<traci_server>` `<remote-port value ="8873"/>` `</traci_server>`) is added to the current  **hello.sumocfg** file. 
 This part is to create a connection to Matlab. Also for the current scenario, we have considered 3 car in the straight road with single lane. 
 So **hello.rou** file need to be change as given below:
 
 `<?xml version="1.0" encoding="UTF-8"?>`
`<routes xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/routes_file.xsd">`
`<vType accel="1.0" decel="5.0" id="Car" length="5.0" minGap="2.0" maxSpeed="50.0" sigma="0" />`
`<route id="route0" edges="1to2 out" />`
`<vehicle depart="0" id="veh0" route="route0" departPos="random" departSpeed="random" type="Car" />`
`<vehicle depart="0" id="veh1" route="route0" departPos="random" departSpeed="random" type="Car" />`
`<vehicle depart="0" id="veh2" route="route0" departPos="random" departSpeed="random" type="Car" />`
`</routes>`


Once this is done, SUMO is ready to make a connection with Matlab.
 
<figure align="center">
  <img src="{{ site.urlbase }}images/crets_sumo_ns3_2.png" width="800">
 </figure>  


<figure align="center">
  <img src="{{ site.urlbase }}images/crets_sumo_ns3_matlab.png" width="800">
 </figure>  




## Relevant Publications
[1] Amr Ibrahim, Chetan Belagal Math, Dip Goswami, Twan Basten, and Hong Li. "Co-simulation framework for control, communication and traffic for vehicle platoons." In 2018 21st Euromicro Conference on Digital System Design (DSD), pp. 352-356. IEEE, 2018.  

[2] Amr Ibrahim, Mladen Čičić, Dip Goswami, Twan Basten, and Karl Henrik Johansson. "Control of Platooned Vehicles in Presence of Traffic Shock Waves." In 2019 IEEE Intelligent Transportation Systems Conference (ITSC), pp. 1727-1734. IEEE, 2019.

## Note
The version that is 
tested for this framewrok can be downloaded from  [here]({{ site.urlbase }}files/traci4matlab.zip).

## Developers
Amr Ibrahim <br/>
[Sajith Muhammed](https://www.linkedin.com/in/sajith-muhammed-84669867) <br/>
Gaurav Pathak <br/>
Chetan Belagal Math

## Contact
[a.ibrahim@tue.nl](mailto:a.ibrahim@tue.nl) <br/>
[d.goswami@tue.nl](mailto:d.goswami@tue.nl)
