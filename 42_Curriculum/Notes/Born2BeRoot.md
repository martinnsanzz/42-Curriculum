# Born2BeRoot

<h2>
	<u>Concept / What it is</u>
</h2>

A system administration project. You set up a minimal Linux server inside a VM, configure it securely, and defend your choices. No GUI — server only.

**VM options:** VirtualBox (preferred) | UTM (Apple Silicon fallback)
**OS options:** Debian (recommended for beginners) | Rocky Linux (more complex)

> **Snapshots are FORBIDDEN.** Grade = 0 if detected.
> **No graphical server** (X.org, Wayland, etc.). Grade = 0 if installed.

---

<h2>
	<u>Submission</u>
</h2>

- Submit only `signature.txt` at the root of your Git repo.
- Contains the SHA1 hash of your `.vdi` (VirtualBox) or `.qcow2` (UTM) disk file.
- **Never push the VM itself.**

```bash
# Linux
sha1sum your_machine.vdi

# Mac (UTM)
shasum your_machine.utm/Images/disk-0.qcow2
```

> VM signature changes after each evaluation boot. Duplicate the VM or use save state before evaluations.
---

<h2>
	<u>Tasks Breakdown</u>
</h2>

1. [ ] Install VirtualBox and create VM (Debian recommended).
2. [ ] Install Debian — no desktop environment, no extra packages.
3. [ ] Set up encrypted LVM partitions (min 2).
4. [ ] Enable and configure AppArmor (`aa-status`).
5. [ ] Configure SSH on port 4242, disable root login (`/etc/ssh/sshd_config`).
6. [ ] Configure UFW — allow only port 4242.
7. [ ] Set hostname to `<login>42`.
8. [ ] Create user `<login>`, add to `user42` and `sudo`.
9. [ ] Configure password policy (`/etc/login.defs` + `pam_pwquality`).
10. [ ] Configure sudo rules (`/etc/sudoers.d/`), create `/var/log/sudo/`.
11. [ ] Write `monitoring.sh` and schedule via `cron` every 10 min.
12. [ ] Change all passwords to comply with new policy.
13. [ ] Generate SHA1 of `.vdi` → paste into `signature.txt`.
14. [ ] Write `README.md` with all required sections.
15. [ ] [TODO] Bonus: extended partitions + WordPress + extra service.

---

# Learnings

<h2>
	<u>Virtual Machines</u>
</h2>

A virtual machine emulates a physical computer, running its own operating system and apps with virtualized resources. It’s isolated from the host system, allowing users to perform secure tasks like testing apps or using different operating systems while optimizing physical hardware.

**Key Features of using a VM**

- Virtual machines behave like an actual computer running on a separate environment or operating system (OS).
- VMs can offer better resource deployment, isolation of applications, and enhanced security.
- VMs can also drive innovation, allowing seamless integration with emerging technologies like AI and edge computing.

**How does it work?**

Virtualization is a complex process through which a software-based, or virtual version of a computer is created. This virtual machinery is allocated specific portions of CPU, memory, and storage from a physical host computer, such as your personal device or a remote server located in a cloud provider's datacenter.  

A VM is a computer file, often referred to as an image, that functions like a physical computer. It operates in a window as an independent computing environment, often to run a different OS or act as the user's entire computer experience. The VM is also isolated from the rest of the system, which means that the software within the VM can’t interfere with the host computer's primary OS.


<h2>
	<u>Operating systems</u>
</h2>
An operating system consists of several fundamental programs that a computer needs to communicate with and receive instructions from users, such as reading and writing data to the hard drive, tapes, and printers; controlling memory usage; and running other programs. The most important part of an operating system is the kernel. In a GNU/Linux system, the kernel is Linux. The rest of the system consists of other programs, many of which were written by or for the GNU project. Since the Linux kernel itself does not form a fully functional operating system, we prefer to use the term " GNU/Linux " to refer to the systems that most people informally call Linux.

GNU/Linux is modeled after a Unix-like operating system. From its inception, GNU/Linux was designed to be a multitasking, multi-user system.

**Unix systems**

Unix(officially registered as UNIX®) is a portable, multitasking, multi-user operating system Its design is based on the philosophy of providing simple and modular tools that can be combined using pipes to perform complex tasks, standing out for its stability, security and ability to run in various hardware environments

Most modern systems derived from or inspired by Unix, such as Linux , the BSD variants and macOS/iOS, are not officially certified Unix, but share its interface, file structure and POSIX standards


<h2>
	<u>ISO files</u>
</h2>

An ISO file, often called an ISO image, is a single file that's a perfect representation of an entire CD, DVD, or BD. The entire contents of a disc can be precisely duplicated in a single ISO file.

ISO images are often used to distribute large programs over the internet, due to the fact that all of the program's files can be neatly contained as a single file. ISO files can contain a full Linux OS, containing the installer, base system and packages.


<h2>
	<u>AppArmor</u>
</h2>

Is an effective Linux application security system that protect the OS and apps from external or internal threats

Many Linux distributions such as Debian or Ubuntu come with AppArmor by default

```bash./
$ sudo aa-status
apparmor module is loaded.
```
>Run aa-status to see if your Linux distribution already has AppArmor integrated
Since it is a kernel module it is usually not something users install themselves. Individual users and system administrators might however want to manage the application profiles which define what each application is allowed to do by editing the files in /etc/apparmor.d/

**Check AppArmor log messages**
Each time AppArmor denies applications from doing potentially harmful operations the event is logged. Depending on your system the AppArmor events can be seen in (syslog, auditd, kernel log or journald logs)

Example:

```bash
$ sudo journalctl -fx
audit[13172]: AVC apparmor="ALLOWED" operation="open"
profile="libreoffice-soffice"
name="/home/otto/.mozilla/firefox/ov37570l.default/cert8.db"
pid=13172 comm="soffice.bin" requested_mask="w"
denied_mask="w" fsuid=1001 ouid=1001
Desktop systems that have the tool aa-notify installed can show events as graphical notifications.
```

<h2>
	<u>Encripted Partitions</u>
</h2>

Encrypting files, partitions, and entire disks prevents unauthorized access to your data and protects your confidential files and documents.

**Disk Partitions**

Disk Partitioning is the process of dividing a disk into one or more logical areas, often known as partitions, on which the user can work separately.

Partitioning is used for:​
- To upgrade the hard Disk (to incorporate a new Hard Disk into the system)​.
- Dual Booting (Multiple Operating Systems on the same system)​.
- Efficient disk management​.
- Ensure backup and security​.
- Work with different File Systems using the same system​.

**Encrypted vs Not Encripted**

Standard partitions is an unencrypted logical section of a hard drive that stores the data
on a `readable format` whereas an encripted particion uses cryptographic alghoritms to render its contents unreadable without a key or a password

<h2>
	<u>LVM</u>
</h2>

LVM is an implementation of a logical volume manager for the Linux Kernel.

It includes many features from a volume manager such as:
- Resizing logical groups
- Resizing logical volumes

Logical volume management provides a high-level view of storage on a computer instead of the traditional view of disks and parititons.

Storage volumes under LVM control can be resized and moved at will, although this may require updating system tools.

LVM also allows management in user-defined groups, allowing the system administrator to deal with volumes named, for example, "sales" or "development", instead of physical device names, such as "sda" or "sdb".

<h2>
	<u>SSH</u>
</h2>

It stands for Secure Shell. he SSH protocol allows Linux administrators to log in to any number of remote systems from their own command line terminal. SSH is a client-server service providing secure, encrypted connections over a network connection. This allows us terminal access to other Linux systems or really any device that accepts SSH connections, such as routers and firewalls, and other operating systems.

**Ports**

A port is just a number (0–65535) that tells the OS which process gets incoming network traffic.

**Common Ports**

| Port | Protocol   | What it does                  |
|------|------------|-------------------------------|
| 22   | SSH        | Encrypted remote shell        |
| 80   | HTTP       | Web traffic, unencrypted      |
| 443  | HTTPS      | Web traffic, encrypted        |
| 21   | FTP        | File transfer (old, avoid)    |
| 25   | SMTP       | Email sending                 |
| 5432 | PostgreSQL | Database                      |
| 3306 | MySQL      | Database                      |

Running SSH on a non-standard port (e.g. 2222) reduces noise from automated bots that scan port 22 constantly. It's not real security — just reduces log spam.

**Open SSH**

OpenSSH is the premier connectivity tool for remote login with the SSH protocol. It encrypts all traffic to eliminate eavesdropping, connection hijacking, and other attacks. In addition, OpenSSH provides a large suite of secure tunneling capabilities, several authentication methods, and sophisticated configuration options.


<h2>
	<u>UFW</u>
</h2>

A firewall is a network security system or device that monitors and controls incoming and outgoing network traffic based on a predefined set of security rules.  Acting as a barrier between a trusted internal network and untrusted external networks, it functions like a gatekeeper that allows, blocks, or drops data packets to prevent unauthorized access and protect against cyber threats.

UFW (Uncomplicated Firewall) is a frontend for iptables.
- iptables is the actual Linux kernel firewall — powerful but complex to configure directly. 
UFW simplifies it into human-readable commands.

To install UFW `sudo apt install ufv` and then enable it `sudo ufw enable`

<h2>
	<u>Visudo</u>
</h2>

Visudo is the safe command-line utility used to edit the /etc/sudoers file on
Linux and Unix systems, which defines which users can run commands with 
elevated privileges.
It prevents system lockouts by locking the file against simultaneous edits, performing
syntax validation before saving, and rejecting changes if parse errors are detected.

Safety Checks: Unlike standard text editors, visudo validates the sudoers syntax 
upon exit; if an error is found, it prompts the user to fix it or discard changes, 
ensuring sudo functionality is not broken.

**Sudo Configuration**
To modify sudo polocies we need to first create a sudo config file in the directory
`/etc/sudoers.d/sudo_config`

And to store the sudo commands logs a directory inside `/var/log/sudo`

Inside the config file we write down the rules (Based on Born2BeRoot):

```bash
Defaults  passwd_tries=3 #Max password attempts before sudo fails
Defaults  badpass_message="Wrong password" #Custom error message on wrong password
Defaults  logfile="/var/log/sudo/sudo_config" #Where sudo commands log are stored
Defaults  log_input, log_output #Logs both whats type and returned
Defaults  iolog_dir="/var/log/sudo" #Directory for input/output logs
Defaults  requiretty #Sudo can only run from a real terminal not scripts
Defaults  secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/snap/bin" #Directories sudo searches for commands
```

<h2>
	<u>Passwords Policies</u>
</h2>

Password polocies are enforced through two main layers: `PAM (Pluggable Authentication Modules)` and login.defs

Linux PAM is a suite of libraries that allows system administrators to configure user authentication methods centrally, 
without modifying application source code

1. /etc/login.defs - System-wide defaults
Controls aging and length for new accounts

```bash
PASS_MAX_DAYS 90 # password expires after 90 days
PASS_MIN_DAYS   # minimum days before changing again
PASS_WARN_AGE 7  # warn 7 days before expiry
PASS_MIN_LEN  8  # minimum length (weak enforcement)
```
These only apply at account creation. For existing users, use chage.

`Aging:` time-based rules that control when a password must be changed
To check aging for a user `chage -l username`

Output:
Last password change        : Jan 01, 2025
Password expires            : Apr 01, 2025
Password inactive           : never
Account expires             : never
Minimum number of days      : 1
Maximum number of days      : 90
Number of days of warning   : 7

2. /etc/pam.d/common-password
The module pam_pwquality enforces complexity. Options specified here can override the configuration file.

For this we first need to install the password quality library
```bash
sudo apt install libpam-pwquality
```

```bash
minlen=10 # The minimum characters a password must contain.
ucredit=-1 # The password must contain at least one capital letter.
dcredit=-1 # The password must contain at least one digit.
lcredit=-1 # The password must contain at least one lowercase letter.
maxrepeat=3 # The password cannot have the same character repeated three consecutive times.
reject_username # The password cannot contain the username within itself.
difok=7 # The password must contain at least seven different characters from the last password used.
enforce_for_root # We will implement this password policy for root.
```

---

# Commands

1. **su - Switch to root user:** The `su` command (substitute user or switch user) is a Unix and Linux utility that allows you to change the identity of the current user and execute commands with the privileges of another user, the most common being the root user (administrator).

The general syntax is if no user is specified, the system assumes that you want to access the root account `su [opciones] [usuario]`. It will then ask for root password.

```bash
su
```

To switch into another user the following command would be use:

```bash
su - username
```

2. **apt - Advance Package Tool:** The `apt` is a command-line utility for installing, updating, removing, and otherwise managing deb packages on Ubuntu, Debian, and related Linux distribution

Most of the apt commands must be run as a user with sudo privileges.

*Most common `apt` commands:*
- sudo apt update: This will pull the latest changes from the APT repositories
- sudo apt intall {package_name}: Installing packages.
- sudo apt upgrade: It upgrades the installed packages to their latest versions
- sudo apt full-upgrade: It will remove installe package if its needed to upgrade the full system
- sudo apt intall --reinstall: Use it if a package is broken or to restore files

```bash
apt install sudo
```

In order to update the system, the server needs to be up to date. If the system clock is behind
the current real time the APT refuses the update.

> Not live until 2026-05-18T08:45:10Z → your clock is before that date

Vm's dont have a real hardware clock, to fix it and sync the time automatically we need to
install the following package:

```bash
sudo apt install ntpsec-ntpdate
sudo ntpdate pool.ntp.org #Use this command to sync time automatically
date #This command shows you the current time on the VMs (Should be the real time)
timedatectl status #Shows you in detail time information
```

3. **sudo commands**:

*sudo reboot*: Is the most direct way of restarting the system immediatly from the terminal. It requires sudo privileges. This command sends signals to all connected processes and users to close files and stop services in an orderly manner before restarting the kernel.

```bash
sudo reboot
```

*sudo adduser:* This command is used to create a new user account with an interactive workflow

```bash
sudo adduser <user_name>
```

*sudo addgroup:* Creates a new group. This groups are permissions systems, so instead of giving permission to users one by one you create a group with the permission you want and add the users to the group. One example is a sudo group which will allow users in that group to run sudo commands

```bash
sudo addgroup <group_name>
```

To check all the groups that the system contains you need to check `etc/group` file. Everytime a new user is created a new group with a new `GID: Group ID` is created (e.g., user_name:x:1000)

To add a user to a group we user the `adduser` command with the following syntax:

```bash
sudo adduser <user_name> <group_name>
```

To remove a user from a group use `gpasswd` command which is a lower-level tool that manages group passwords and membership. We could use `deluser` but is high-level tool which may not be available

```bash
sudo gpasswd -d <user_name> <group_name>
```

To check groups and the users inside them we use `getent` which reads databases like `/etc/group`

```bash
getent group
```

For specific groups

```bash
getent group <group_name> <group_name> ...
```

To check all users on the server. We use the command `tail -10`, since non-system users
are listed on the bottom in descending order of creation.

```bash
cat /etc/passwd | tail -10
```

To delete a user completely (user + home directory)

```bash
sudo userdel -rf <user_name>
```


4. **systemctl:** Is the primary utility function for controlling systemd system and service
manager in Linux. The most commun commands are start, stop, restart, enable, disable, and
inspect.

```bash
systemctl <command> <service>
```

To check which services are enabled on startup run

```bash
sudo systemctl list-unit-files --state=enabled
```


5. **ssh commands:** It provides secure encrypted communication between local and remote
Linux/Unix systems.

To connet to a local machine via SSH with a specific port number

```bash
ssh username@localhost -p 4242
```

To check the SSH service status

```bash
sudo service ssh status
```

To start, stop and restart

```bash
sudo service ssh start
sudo service ssh stop
sudo service ssh restart
```

To check SSH is running after reboot

```bash
sudo service ssh enable
```

6. **UFW commands:** This commands are user friendly and build on top of iptoables which is the firewall from the kernel

*Basic Management*

```bash
sudo ufw status #Check status and shows active rules
sudo ufw status verbose #Includes logging levels and defaults
sudo ufw enable / disable #Activates or deactivates the firewall
sudo ufw reload #Apply changes
sudo ufw reset #Remove all rules and return to defaults
```

*Creating and managing rules*

```bash
sudo ufw allow <port_number> #Allows incoming connection on the specified port
sudo ufw delete <port_index> #Deletes port connection at that index (check ufe status)
```

7. **passwd:** This command is used to change th users passwords. It updates the hashed entry
in /etc/shadow.

```bash
passwd #Changes the password of the current user
sudo passwd <username> #Changes another user's password
```

>> If your in root user, it wont ask for the current password

*What it does*
1. Prompts for the new password (twice for confirmation)
2. Checks it against the password policy (/etc/pam.d/common-password)
3. Hashes it with SHA-512
4. Writes the hash to /etc/shadow

Polocy enforcement is configured in:
- /etc/login.defs         # Aging rules (MAX_DAYS, MIN_DAYS, WARN_AGE)
- /etc/pam.d/common-password  # Complexity rules (length, uppercase, etc.)

8. **chage:** Used to chang the user password aging. It controls how long a password is valid
and when the user must change it. It reads and writes to /etc/shadow

```bash
sudo chage <username> #Interactive mode (it prompts for each value)
sudo chage -l <username> #List the current aging info
sudo chage -M 30 <username>    # Max days before password must change
sudo chage -m 2 <username>     # Min days before password CAN change
sudo chage -W 7 <username>     # Days of warning before expiry
sudo chage -E 2025-12-31 <username>  # Account expiry date
sudo chage -d 0 <username>     # Force password change on next login
```

# Script

First line of the script needs to be the shebang `#!` followed by the path of the command-line
interpreter or scripting language (In this case bash) `/bin/bash`

>The file needs to have execussion permissions `chmod u+x <file_name>`

```bash
#!/bin/bash
```

**Shebang:** It serves as an interpreter directive, telling the OS program loader which
executale should be used to interpret and run the script

### OS Architecture and Kernel
To get the architecture of the OS and the Kernel version you use the command `uname -a`.
It prints all the information related to the OS

### Physical Cores
Cpu information is primarily stored in `/proc/cpuinfo` file. This is a read-only plain text file
and provides snapshots of the CPU's current state.

To get the physical cores number we use the command `grep "physical id" /proc/cpuinfo | wc -l`

**grep command**
It looks inside the file `/proc/cpuinfo` and searches for each line containing `"physical id"`.
Each logical CPU core has one of this lines.

**| wc -l**
It counts the numbers of lines piped into it.
>wc stands for (word count), flag -l count only for the number of newline characters

### Virtual Cores
We use the same previous command but instead of looking for the `physical id` we look for the
`processor`

### Ram memory
Too display the ram information in this format `74/987MB (7.50%)` we need 3 variables.
1. Ram total: `free --mega | awk '$1 == "Mem:" {print $3}'`
2. Ram use: `free --mega | awk '$1 == "Mem:" {print $2}'`
3. Ram percentage: `free --mega | awk '$1 == "Mem:" {printf("(%.2f%%)\n", $3/$2*100)}'`

**free** This command display the memory usage (RAM + swap)

```bash
free --mega #Shows the values in megabytes
```

>               total    used    free    shared    buff/cache    available
>Mem:           8192    2048    4096       128          2048         5900
>Swap:          2048       0    2048

Each column is a field, the command `awk` references them by position $1, $2, $3...
$1 = "Mem:", so this condition is true and runs the action
$2 = total memory 'prints 8192'
$3 = used memory 'prints 2048'

**Mental mode**
free --mega        →  produces a table
awk '$1=="Mem:"'   →  filters to the Mem row only
{print $3}         →  grabs the column you want

### Disk usage
Too display the disk information in this format `1009/2Gb (49%)` we need 3 variables.
1. Storage total: `df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_t += $2} END {printf ("%.1fGb\n"), disk_t/1024}'`
2. Storage use: `df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_u += $3} END {print disk_u}'`
3. Storage percentage: `df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_u += $3} {disk_t+= $2} END {printf("%d"), disk_u/disk_t*100}'`

**df:** This command shows disk space usage per filesystem. The flag `-m` displays this
information in megabutes

>Filesystem     1M-blocks  Used  Available  Use%  Mounted on
>/dev/sda1          20000  5000      15000   25%  /
>/dev/sda2           1000   200        800   20%  /boot

The pipeline logic (same pattern for all 3):
```bash
df -m | grep "/dev/" | grep -v "/boot" | awk '...'
```
- grep "/dev/" -> Keep only real disk partitions
- grep -v "/boot" -> Exclude the /boot 
- awk '...' -> Process the remaining lines

### CPU usage
To display the usage of the CPU in percetenage we will work with the command `vmstat` which shows system statistics in the following format

>procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
> r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
> 3  0      0 6803828  44620 4327124    0    0    34    29  245  339  3  1 96  0  0

We will then only print the word 15 on the last line `tail -1` which is the cpu usage

```bash
vmstat | tail -1 | awk '{print $15}'
```

To then get the % of the CPU load we need to do `100 - cpu_load` and print `%.1f`

```bash
cpul=$(vmstat 1 2 | tail -1 | awk '{printf $15}')
cpu_op=$(expr 100 - $cpul)
cpu_fin=$(printf "%.1f" $cpu_op)
```

>expr command is used to evaluate expressions and do arithmetic operations

### Last boot
To get the time and date of the last system reboot we will use the command `who` which displays
information of users who are corrently logged in. We will use the flag `-b` to get the date and
time of the last system boot and use awk to only get the information we want

```bash
who -b | awk '$1 == "system" {print $3 " " $4}'
```

>2021-04-25 14:45

### LVM usage
To check if the system is using lvm partitions we will use th ecommand `lsblk` which displays
all information about all available block devices. We will pipe this into `grep "lvm"` and check
if the return is bigger than 0 which means that lvm is being used.

```bash
lvmu=$(if [ $(lsblk | grep "lvm" | wc -l) -gt 0 ]; then echo yes; else echo no; fi)
```

### Number of active connections
To check how many connections our server has we use the command `ss` with the flags `-ta`
- t -> Displays only TCP sockets
- a -> all

Then we pipe this to `grep = ESTAB` which means established and display the number of lines it
returns with `wc -l`

```bash
ss -ta | grep ESTAB | wc -l
```

### Number of users
We will use the command `users` and count how many there are with `wc -w`.
>The flag -w count the number of words, instead of -l which counts number of lines

### IP adress & MAC
For this we will use 2 commands.

- For the IP address `hostname -I` which will print allthe IP addresses for the host and we will
only print the first word
- For the MAC (Media Access Control) address we will use the command `ip link` and get 
the `link/ether` line

### Number of sudo commands
To get this number we will use the command `journalctl` which is a tools that displays the logs
from systemd-journald.

To get the logs of a specific command we put the field modifier `_COMM=sudo`

```bash
journalctl _COMM=sudo | grep COMMAND | wc -l
```

### Crontab
This command is used in Linux to create, edit and manage scheduled tasks (cron jobs) that run
automatically at specified times or intervals.

To check if crontab is active on the system run:

```bash
systmctl status cron
```

To configure crontab we need to edit the crontab file with the following command

```bash
sudo vim /etc/crontab
```

Inside this file we need to specify which actions needs to be done and every how long

```bash
# m  h  dom  mon  dow  USER    command
*/10 *  *    *    *    root    /home/masanz-s/monitoring.sh
```

### Broadcasting wall across SSH Sessions
Wall only broadcasts to terminals registered in `/var/run/utmp`. SSH sessions
do not appear in this file by default

The main files that need to be created fo this to work are:
- /var/run/utmp -> It tracks active login sessions
- /etc/tmpfiles.d/utmp.conf -> Recreates utmp on reboot

Set up permission of `utmp` file
```bash
sudo touch /var/run/utmp
sudo chmod 664 /var/run/utmp
sudo chown root:utmp /var/run/utmp
```

Inside utmp.conf write `f /var/run/utmp 0664 root utmp -` which forces the permissions
on the file within the path on reboot


### Final Script
```bash
#!/bin/bash

#OS architecture and Kernel version
arch=$(uname -a)

#Number of physical processors
phy_proc=$(grep "physical id" /proc/cpuinfo | wc -l)

#Number of virtual processors
vir_proc=$(grep "processor" /proc/cpuinfo | wc -l)

#Current available RAM
ram_total=$(free --mega | awk '$1 == "Mem:" {print $2}')
ram_use=$(free --mega | awk '$1 == "Mem:" {print $3}')
ram_percent=$(free --mega | awk '$1 == "Mem:" {printf("%.2f"), $3/$2*100}')

#Current available storage
stor_total=$(df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_t += $2} END {printf ("%.1f"), disk_t/1024}')
stor_use=$(df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_u += $3} END {print disk_u}')
stor_percent=$(df -m | grep "/dev/" | grep -v "/boot" | awk '{disk_u += $3} {disk_t += $2} END {printf("%d"), disk_u/disk_t*100}')

#CPU usage
cpu_load=$(vmstat | tail -1 | awk '{print $15}')
cpu_op=$(expr 100 - $cpu_load)
cpu_end=$(printf "%.1f" $cpu_op)

#Last boot
last_boot=$(who -b | awk '$1 == "system" {print $3 " " $4}')

#LVM Status
lvm=$(if [ $(lsblk | grep "lvm" | wc -l) -gt 0 ]; then echo yes; else echo no; fi)

#Number of active connections
ac_connections=$(ss -ta | grep ESTAB | wc -l)

#Number of users using the server
users_num=$(users | wc -w)

#IP address of server and MAC
ip=$(hostname -I | awk '{print $1}')
mac=$(ip link | grep "link/ether" | awk '{print $2}')

#Number of sudo commands
sudo_cmnd=$(journalctl _COMM=sudo | grep COMMAND | wc -l)

wall "
		#Architecture: $arch
		#CPU physical: $phy_proc
		#vCPU: $vir_proc
		#Memory usage: $ram_use/${ram_total}MB ($ram_percent%)
		#Disk usage: $stor_use/${stor_total}GB ($stor_percent%)
		#CPU load: $cpu_end%
		#Last boot: $last_boot
		#LVM use: $lvm
		#Connections TCP: $ac_connections ESTABLISHED
		#User log: $users_num
		#Network: IP $ip ($mac)
		#Sudo: $sudo_cmnd cmd"
```

# Bonus

## Partions
A partition is a logical division of a physical disk. This disk is one physical
device, but you can split it into separate sections that the OS treats independently.

```visual_mode
Physical disk (15GB)
├── sda1 (500MB)  → /boot
├── sda2 (1KB)    → Extended container
└── sda5 (14.5GB) → LVM
```

### Mount Points
A mount point is a directory where a partition becomes accesible. Instead of `C:` 
or `D:` like Windows, Linux attaches partitions to folders.

```visual_mode
sda1  →  mounted at  /boot
sda5  →  mounted at  /home
```
Everything in Linux is one big directory tree - partitions just "plug in" at different
point of the tree.

### LVM (Logical Volume Manager)
LVM adds a flexible layer between physical partitions and the filesystem. Instead of 
fixed partitions you can resize, add, and manage volumes dinamically.

```visual_mode
Physical disk
└── sda5 (physical partition)
    └── sda5_crypt (encrypted container)
        └── LVMGroup (volume group)
            ├── root    (10GB) → /
            ├── swap    (2GB)  → [SWAP]
            ├── home    (5GB)  → /home
            └── var-log (4GB)  → /var/log
```

[SWAP] is not a real mountpoint — it's extra virtual RAM. When RAM is full, the system 
moves inactive data here temporarily. Slow but prevents crashes.

There are 3 levels to an LVM:
### LVM Layers

| Layer | What it is |
|---|---|
| **PV** (Physical Volume) | The real partition given to LVM (`sda5`) |
| **VG** (Volume Group) | Pool of all PV space combined (`LVMGroup`) |
| **LV** (Logical Volume) | Virtual partitions carved from the VG (`root`, `home`, etc.) |

### Encryption (LUKS)
In the subject, `sda5_crypt` appears between the partition and LVM. This means the entire 
LVM lives inside an encrypted container — you enter a password at boot to unlock it before 
the system can access any volume.

## Setting up Wordpress website
WordPress is a CMS (Content Management System) — software that lets you create and manage
a website without writing code from scratch. It powers about 40% of all websites on
the internet.
You write content (posts, pages) through a web interface, and WordPress handles displaying
it to visitors.

**How it works**
> Visitor browser → Web server → PHP → Database → back to browser
1. Visitor requests a page
2. Web server (lighttpd) receives the request
3. PHP processes the WordPress code
4. WordPress queries the database (MariaDB) for content
5. PHP builds the HTML page and sends it back
6. Visitor sees the website

**The 3 requirements**

| Service | Role | Why needed |
|---|---|---|
| **lighttpd** | Web server | Receives HTTP requests, serves files |
| **PHP** | Scripting language | WordPress is written in PHP, can't run without it |
| **MariaDB** | Database | Stores all content, users, settings |

**How to install WordPress**
To be able to install the lates version of WordPress we need to install `wget` and `zip`.
- wget is a command line toold used to download files from the web
- zip is a command line utility for compressing and decompressing files in ZIP format

To download the latest version of WordPress use the following link:
`https://wordpress.org/latest.zip`. 
Move to the `/var/www/html` folder and use the link with the command `wget` with sudo 
permission.

Once the .zip file is downloaded we need to unzip it itht the command `unzip`. Finally we will
change the permissions to the wordpress folder to `755` (Owner can rwx, and group and others
can rx)

```bash
cd /var/www/html
sudo wget https://wordpress.org/latest.zip
sudo unsip latest.zip
sudo chmod -R 755 wordpress 
```

### Content Management System (CMS)
A CMS is software that lets you create, edit, and manage website content through a 
graphical interface — without needing to write HTML, CSS, or code manually.

**How does it work**
- Without a CMS: Every time you want to add a blog post you'd manually write HTML, 
upload files, update navigation links — all by hand.
- With a CMS: You log into a dashboard, write your post like a Word document, click publish. 
The CMS handles everything else.


A CMSs is divided in two parts:
- The first one is the CMA (Content management application), which
is the part that allows the user to add and makes changes to the website.

- The second one is the back-end process CDA (Content delivery application) which takes the 
input to the CMA and stores it behind the scenes, making it visiable for all site visitors.

This two parts work together so organizations no longer need to handle code and database queries
manually. They can focus on the front-end content.

**Examples of a CMS**
There are many different CMS options and they all have their own purpuse relevant to their own
features. Most known examples are:

- WordPress: Originally was a web content management system that was built to publish blogs, 
but has extended into many other areas. The open source management system can be used 
for websites, professional portfolios, e-commerce stores and more.

- Shopify: This e-commerce platform wouldn’t be able to function without its CMS. The 
platform is built for businesses that want to create online stores. They are then able to 
edit and manage different content types through one software system.

- Wix: The web-based platform is software that creators and businesses use to make and 
manage their own websites without needing to know how to code. The platform provides 
advanced SEO features and marketing tools.

### Lighttpd
Lighttpd is open-source web server software. Its designed for environments with limited
resources since it consumes minimal CPU and RAM. It suitbale for Windows and Linux (OSs).

Like other web server software, Lighttpd makes the content visible online. When a user's browser
requests to view your website, the software receives the query, retrieves your site's content
and displays it in HTML format.

**How to install Lighttd**
To install lighttpd correctly first we need to make sure the system is up to date and working
as it should

```bash
sudo apt update
sudo apt upgrade
```

Once system is up-to-date we install lighttpd and enable ot on boot

```bash
sudo apt install lighttpd
sudo systemctl enable lighttpd
```

To check it has being enable and is active we run the following commands

```bash
sudo systemctl is-enabled lighttpd
sudo systemctl status lighttpd
```


### MariaDB
MariaDB is a `general-purpose open-source relational database management system`. It uses the
relational database model which organizes data into tables. These tables can be related to each
other, enabling complex queries and efficient data management. The databases are based on `SQL`
language which is designed for accessing and managing data in relational databases.

MariaDB is programmed in C and C++ but also supports Java, Python, PHP and Perl and can be used
in Windows, Linux and macOS.

**What is MariaDB most suitable for?**
MariaDB is suitable for a wide range of ap­pli­ca­tions, from classic web projects to
data-intensive en­ter­prise solutions and modern cloud and Big Data en­vi­ron­ments. 
It is also ideal for every­thing from simple entry-level projects to complex software 
ar­chi­tec­tures. 

Classic use case would be:

- Web ap­pli­ca­tions: MariaDB is often used to manage data in web ap­pli­ca­tions. An example includes content man­age­ment systems like WordPress, Joomla!, or Drupal, which run reliably on MariaDB despite high traffic volumes.

**How to install MariaDB**
To install lighttpd correctly first we need to make sure the system is up to date and working
as it should

```bash
sudo apt update
sudo apt upgrade
```

Once system is up-to-date we install MariaDB with the following command

```bash
sudo apt install mariadeb-server -y
```
> The flag -y automatically confirms the installation

Once downloaded the MariaDB service should start immediately. To check

```bash
sudo systemctl status mariadb
sudo systemctl start mariadb #If not started automatically
sudo systemctl enable mariadb #Run this to make sure it loads on boot
sudo systemctl is-enabled mariadb #Run this to check if its enable
```

The defualt configuration of MariaDB leaves installation unsecure. To fix it we will use a
script provided by Mariadb-server package to restrict access to the server and remove unused 
accounts. We will run the script with the following command:

```bash
sudo mariadb
```
This will open MariaDB monitor and we will write this:

```sql
DELETE FROM mysql.user WHERE User='';
DELETE FROM mysql.user WHERE User='root' AND Host NOT IN ('localhost', '127.0.0.1', '::1');
DROP DATABASE IF EXISTS test;
DELETE FROM mysql.db WHERE Db='test' OR Db='test\\_%';
FLUSH PRIVILEGES;
```
After writing EXIT; the configuration will be done.

| Command | Purpose |
|---|---|
| Delete empty users | Removes anonymous accounts |
| Delete remote root | Root can only login locally |
| Drop test database | Removes default test DB |
| Flush privileges | Applies all changes |

The most important files/directorys of MariaDB are found in:
- /var/lib/mysql/ -> This is where the WordPress database will be stored
- /etc/mysql -> This is where all the configuration files are 

**How to use MariaDB**
In order to create a database and the user for WordPress we need to access mariadb.
There's two commands for it

```bash
sudo mariadb #No password needed, faster and simpler
mysql -u root -p #It asks for root password
```

Theres multiple commands that we can use with mariadb in sql query language. Most important
command is `quit/exit` to exit mariadb interface and go back to terminal

```sql
CREATE DATABASE <data_base_name>; -- Create a new database
DROP DATABASE <data_base_name>; -- Deletes an existing database database
USE <data_base_name>; -- Connect to a database and select it to use
CREATE USER 'username'@'localhost' IDENTIFIED BY 'password'; -- Creates a new local user with a password
GRANT ALL PRIVILEGES ON <data_base_name>.* TO 'username'@'localhost'; -- Grants the user all privilages on the database
FLUSH PRIVILEGES; -- This updates the permissions and applies the changes 

-- Commands to see things
SHOW DATABASES; -- List all available databases
SELECT User, Host FROM mysql.user; -- See all users
SHOW GRANTS FOR 'username'@'localhost'; -- See privileges for a specific user
```

### PHP
PHP is a popular open-source scripting language created specifically for website design and can
be used in formats including HTML. 
It has unique tags for start and end which you can quickly enter in PHP mode in your HTML code by
using the aboce tags

The execution of PHP occurs on the server-side, rendering its output in HTML for user
interaction. Integrating PHP into HTML is straightforward, requiring only the insertion of `<?`
at the beginning of the PHP segment within the HTML structure. This initiates the PHP 
scripting, which can be concluded with `?>`, signaling the end of PHP code within an HTML document.

>"Server-side" means the code runs on the server, not in the visitor's browser.

**How to install PHP**

To install the PHP packages required by lighttp and MariaDB we do the following command

```bash
sudo apt install php-cgi php-mysql -y
```

- php-cgi is used by Lighttp to run PHP. Lighttpd can't execute PHP directly. It passes PHP
files to a PHP processor via CGI:

>lighttpd → php-cgi → processes PHP → returns HTML → lighttpd serves it

- php-mysql is used by WordPress to talk to MariaDB. WordPress stores everything (posts, users,
settings) in MariaDB. PHP uses the php-mysql module as a bridge to send queries to the 
database and get data back.

## How to configure Wordpress
Once all the previous services have being installed and set-up we need to configure wordpress
to work with all the services.

In the unzip wordpress folder we need to find the `wp-config-sample.php` file and copy it
with the name `wp-config.php` and access it via vim/nano.

Once we are inside the copy of the file we need to edit the database seetings with the new
database we created with MariaDB.

```php
// ** Database settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define( 'DB_NAME', 'database_name_here' ); //Insert name of database here

/** Database username */
define( 'DB_USER', 'username_here' ); //Insert username here

/** Database password */
define( 'DB_PASSWORD', 'password_here' ); //Insert password here

/** Database hostname */
define( 'DB_HOST', 'localhost' ); //Leave like this as is if its local

/** Database charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8mb4' );

/** The database collate type. Don't change this if you doubt. */
define( 'DB_COLLATE', '' );
```

After the wordpress configuration is done, we need to enable the php modules we previously
downloaded 

```bash
sudo lighty-enable-mod fastcgi #This improves performance and speed of web apps on the server
sudo lighty-enable-mod fastcgi-php #Improves performance and speed of PHP-based web apps
sudo service lighttpd force-reload #Updates and apply the changes in the config
```

Once all the setup is done you need to create a new port forwading in the VM settings. 
- For guest use port `8080` 
>(Or any number above 1023 since they are considered privilage port and
>require root which might couse problems)
- For host use port `80`. This is the default for http servers (443 for https)

Once all the settings are applied we go to our browser and type `http://localhost:8080`

This will prompt with the settings of the website and we will need to fill up the blank
boxes with the same information from mariadb for the databases to link. Once filled install
wordpress and you've go the website !!

This commands are usefull:

```bash
sudo systemctl start lighttpd
sudo systemctl stop lighttpd
sudo systemctl restart lighttpd
sudo systemctl status lighttpd
```