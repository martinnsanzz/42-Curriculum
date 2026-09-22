*This project has been created as part of the 42 curriculum by masanz-s*

# Born2BeRoot


## DESCRIPTION
Born2beRoot is a system administration project from the 42 curriculum. The goal is to set up a
secured Linux server inside a virtual machine, following strict configuration rules covering
partitioning, user management, firewall policy, password enforcement, sudo hardening, and
automated monitoring.

The project introduces core sysadmin concepts: LVM encrypted partitions, SSH hardening, UFW firewall rules, PAM-based password policies, sudo logging, and cron-scheduled bash scripting.

## PROJECT DESCRIPTION

### Operating System Choice: Debian

Debian was chosen over Rocky Linux for this project.

| | Debian | Rocky Linux |
|---|---|---|
| Target audience | General purpose, servers, desktops | Enterprise servers (RHEL-compatible) |
| Package manager | `apt` / `aptitude` | `dnf` |
| Security module | AppArmor | SELinux |
| Firewall | UFW | firewalld |
| Learning curve | Lower — better for beginners | Higher — complex SELinux policy management |
| Community & docs | Very large, well-documented | Growing, enterprise-focused |
| Stability | Extremely stable, conservative updates | Stable, tracks RHEL releases |

Debian was selected because it is beginner-friendly, extremely well-documented, and recommended by the subject itself for those new to system administration.

### AppArmor vs SELinux
| | AppArmor | SELinux |
|---|---|---|
| Model | Path-based MAC (Mandatory Access Control) | Label-based MAC |
| Configuration | Profile files per application | Labels on every file and process |
| Complexity | Lower — easier to write and debug profiles | Higher — steep learning curve |
| Default on | Debian, Ubuntu | Rocky, Fedora, RHEL |
| Flexibility | Less granular | More granular and powerful |

AppArmor ships with Debian and must be running at startup. It restricts programs to a defined
set of resources using per-application profiles.

### UFW vs firewalld
| | UFW | firewalld |
|---|---|---|
| Full name | Uncomplicated Firewall | firewalld |
| Backend | iptables / nftables | nftables / iptables |
| Default on | Debian, Ubuntu | Rocky, Fedora, RHEL |
| Interface | Simple CLI (`ufw allow`, `ufw deny`) | `firewall-cmd` with zones |
| Complexity | Very simple | More complex, zone-based |

UFW was used on Debian. Only port 4242 is open for SSH access.

### VirtualBox vs UTM
| | VirtualBox | UTM |
|---|---|---|
| Platform | Windows, Linux, macOS (Intel) | macOS (Apple Silicon — M1/M2) |
| Architecture emulation | x86/x86_64 | x86_64 via QEMU, ARM native |
| Performance on Apple Silicon | Poor (no native support) | Good (native ARM or QEMU) |
| Use case | Standard 42 workstations | Mac M1/M2 users |

VirtualBox was used for this project.

---

## INSTRUCTIONS

### Accessing the Virtual Machine

Connect via SSH from the host machine:

```sh
ssh masanz-s@localhost -p 4241
```

Root login over SSH is disabled. Use `sudo` once logged in as `masanz-s`.

### Verifying Key Services

**AppArmor status:**
```sh
sudo aa-status
```

**UFW status:**
```sh
sudo ufw status
```

**SSH listening on port 4242:**
```sh
ss -tunlp | grep 4242
```

**Sudo log:**
```sh
sudo ls /var/log/sudo/
```

**LVM partitions:**
```sh
lsblk
```

### monitoring.sh

The script runs automatically via `cron` every 10 minutes and broadcasts system information to all terminals using `wall`. It reports: OS architecture and kernel version, physical and virtual CPU count, RAM usage, disk usage, CPU load, last reboot time, LVM status, active TCP connections, logged-in users, IPv4 and MAC address, and the total number of `sudo` commands executed.

**View the cron job:**
```sh
cat /etc/crontab
```

**Run manually:**
```sh
sudo ./monitoring.sh #Need to inside /home/masanz-s
```

**Interrupt without modifying (disable the cron job):**
```sh
sudo systemctl stop cron
```

### Submission

The repository contains only `signature.txt` at its root. This file holds the SHA1 hash of the virtual disk image.

**Generate the signature (Linux):**
```sh
sha1sum ~/sgoinfre/Martin/VMs/Born2BeRoot_Bonus/Born2BeRoot_Bonus.vdi
```

### Bonus: WordPress

The WordPress site runs on `lighttpd` + `MariaDB` + `PHP`. Access it via the VM's IP address on port 8080 (UFW rule added for bonus).

### Bonus: Minecraft Server

A Minecraft Java Edition server runs as an extra service. It listens on port 25565 (UFW rule
added). The service was chosen for its practical value as a long-running, resource-monitored
background process — useful for demonstrating server management, port configuration, and
firewall rule adaptation in a real-world context. 
Note: this service was configured and tested on a personal machine at home, as the software
required to run a Minecraft server is not available on the 42 workstations.

---

## RESOURCES

[Oracle Virtual Box](https://www.virtualbox.org/)

[Oracle Technical Documentation](https://www.virtualbox.org/wiki/Technical_documentation)

[Virtual Box Manual](https://www.virtualbox.org/manual/ch01.html#gui-machine-list)

[AppArmor Documentation](https://apparmor.net/)

[What is an ISO file](https://www.lifewire.com/iso-file-2625923)

[An Introduction to Disk Partitions](https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/5/html/installation_guide/ch-partitions-x86)

[Born2BeRoot guide](https://noreply.gitbook.io/born2beroot)

[How to use cron in Linux](https://opensource.com/article/17/11/how-use-cron-linux)

[Bash Tutorial](https://www.w3schools.com/bash/index.php)

[Arch Linux Wiki](https://wiki.archlinux.org/title/Main_page)

[Content Management System](https://www.ibm.com/think/topics/content-management-system)

[What's MariaDB](https://www.ionos.com/digitalguide/server/know-how/what-is-mariadb/)

[How to install MariaDB](https://www.ionos.com/digitalguide/hosting/technical-matters/install-mysql-mariadb/)

[HTTP vs HTTPS](https://aws.amazon.com/compare/the-difference-between-https-and-http/)

### AI Usage

Claude (claude.ai) was used during this project for the following support tasks only — no configuration files or scripts were generated by AI:

- **Troubleshooting**: diagnosing errors encountered during VM setup, LVM configuration, and service installation
- **Command lookup**: finding and understanding specific commands to verify project requirements (e.g., `ss`, `lsblk`, `aa-status`, `crontab`)
- **Concept explanation**: understanding how LVM, LUKS encryption, AppArmor profiles, PAM modules, and sudo policy enforcement work at a low level
- **Learning support**: clarifying differences between tools (UFW vs firewalld, AppArmor vs SELinux, `apt` vs `aptitude`) to build genuine understanding ahead of the defense