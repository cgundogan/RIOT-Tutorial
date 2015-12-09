# IEEE WF-IOT 2015 Tutorial

## 0. Prerequisites

### Recommended Setup (Using a Virtual Machine)
* bring your laptop
* if absent, install and set up [git](https://help.github.com/articles/set-up-git/)
* if absent, install [VirtualBox](https://www.virtualbox.org/wiki/Downloads)
* if absent, install [VirtualBox Extension Pack](https://www.virtualbox.org/wiki/Downloads)
* if absent, install [Vagrant](https://www.vagrantup.com/downloads.html)
* git clone https://github.com/cgundogan/RIOT-Tutorial.git
* git clone https://github.com/RIOT-OS/RIOT.git
* run the [Vagrant RIOT Setup](https://github.com/RIOT-OS/RIOT/blob/master/dist/tools/vagrant/README.md)

### Advanced Setup (Without Using a VM)
* bring your laptop
* if absent, install and set up [git](https://help.github.com/articles/set-up-git/)
* install the build-essential packet (make, gcc etc.). This varies based on the operating system in use.
* [Native dependencies](https://github.com/RIOT-OS/RIOT/wiki/Family:-native#dependencies)
* [openOCD](https://github.com/RIOT-OS/RIOT/wiki/OpenOCD)
* [GCC Arm Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)
* on OS X: [Tuntap for OS X](http://tuntaposx.sourceforge.net/)
* [additional tweaks](https://github.com/RIOT-OS/RIOT/wiki/Board:-Samr21-xpro) necessary to work with the targeted hardware (ATSAMR21)
* git clone https://github.com/cgundogan/RIOT-Tutorial.git
* git clone https://github.com/RIOT-OS/RIOT.git

### Modifications to Vagrant
For this tutorial we need to make a simple modification to the `Vagrantfile` located in the root
directory of RIOT. Please make sure that the `RIOT-Tutorial` directory is located at the same
level as the `RIOT` directory. Add the following to `line 25` of the `Vagrantfile`:

```
config.vm.synced_folder "../RIOT-Tutorial", "/home/vagrant/RIOT-Tutorial"
```

If you have started the VM before, then you need to issue a `vagrant reload` in order for the change
to take effect.

## 1. Running RIOT on Native
1.  Run `git checkout -f task_1`

2.  First, we need to setup the necessary virtual network interface `tap0`:
    ```sh
    sudo ip tuntap add tap0 mode tap user ${USER}
    sudo ip link set tap0 up
    ```

3.  Change into the **application** directory of this repository and run the following command:
    ```sh
    make all term
    ```

4.  Run `help` to see a list of all available commands.
    ```
    > help
    help
    Command              Description
    ---------------------------------------
    reboot               Reboot the node
    ps                   Prints information about running threads.
    ping6                Ping via ICMPv6
    random_init          initializes the PRNG
    random_get           returns 32 bit of pseudo randomness
    ifconfig             Configure network interfaces
    txtsnd               send raw data
    ncache               manage neighbor cache by hand
    routers              IPv6 default router list
    ```

5.  Run `ifconfig` to see the configured network interfaces and additional information.
    ```
    > ifconfig
    ifconfig
    Iface  4   HWaddr: 4a:ba:26:52:88:dd
               MTU:1500  HL:64  RTR  RTR_ADV
               Source address length: 6
               Link type: wired
               inet6 addr: ff02::1/128  scope: local [multicast]
               inet6 addr: fe80::48ba:26ff:fe52:88dd/64  scope: local
               inet6 addr: ff02::1:ff52:88dd/128  scope: local [multicast]
               inet6 addr: ff02::2/128  scope: local [multicast]
    ```

6.  Copy the [link-local address](https://en.wikipedia.org/wiki/Link-local_address#IPv6) of the
    RIOT node (prefixed with `fe80`, without the `/64`) and try to ping it from your host machine.
    ```
    ping6 fe80::48ba:26ff:fe52:88dd%tap0
    ```
    If this notation does not work, then try the following command:
    ```
    ping6 -I tap0 fe80::48ba:26ff:fe52:88dd
    ```
    It is necessary for link-local addresses to specify the desired interface.

7.  Lookup the link-local address of your `tap0` interface on your host machine:
    ```sh
    > ip addr show dev tap0
    14: tap0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 500
        link/ether 4a:ba:26:52:88:dc brd ff:ff:ff:ff:ff:ff
        inet6 fe80::48ba:26ff:fe52:88dc/64 scope link
           valid_lft forever preferred_lft forever
    ```
    Copy this link-local address (again, without `/64`) and call the following command from within
    the RIOT shell:
    ```
    > ping6 fe80::48ba:26ff:fe52:88dc
    ping6 fe80::48ba:26ff:fe52:88dc
    12 bytes from fe80::48ba:26ff:fe52:88dc: id=84 seq=1 hop limit=64 time = 0.218 ms
    12 bytes from fe80::48ba:26ff:fe52:88dc: id=84 seq=2 hop limit=64 time = 0.234 ms
    12 bytes from fe80::48ba:26ff:fe52:88dc: id=84 seq=3 hop limit=64 time = 0.218 ms
    --- fe80::48ba:26ff:fe52:88dc ping statistics ---
    3 packets transmitted, 3 received, 0% packet loss, time 2.061141 s
    rtt min/avg/max = 0.218/0.223/0.234 ms
    ```

8.  *(Optional)* Create a second tap interface (`tap1`) and start the second instance of RIOT with
    the environment variable `PORT=tap1`. Now you can try to ping between two RIOT instances.

## 2. Running RIOT on Real Hardware (samr21-xpro)
1.  Get to know your hardware

    ![SAMR21-XPRO](SAM-R21.jpg)

    MCU                    | ATSAMR21G18A
    -----------------------|------------------------------------
    Family                 | ARM Cortex-M0+
    Vendor                 | Atmel
    RAM                    | 32Kb
    Flash                  | 256Kb
    Frequency              | up to 48MHz
    FPU                    | no
    Timers                 | 6 (1x 16-bit, 2x 24-bit, 3x 32-bit)
    ADCs                   | 1x 12-bit (8 channels)
    UARTs / SPIs / I2Cs    | max 5 (shared)
    Vcc                    | 1.8V - 3.6V

2.  Run `git checkout -f task_2`

3.  To compile an application for a specific board, we can make use of the `BOARD` environment
    variable.
    ```sh
    BOARD=samr21-xpro make all flash term
    ```
    This command will compile the application, burn the image onto the `samr21-xpro` and open a
    connection to the RIOT shell.

4.  Find out the link-local address that is configured on the radio interface with `ifconfig`.

5.  Ask your neighbors for their link-local addresses and use the `ping6` command to ping other
    nodes. You can specify the amount of ping requests, the delay for each ping request and the
    payload length.
    ```
    ping6 10 <IPv6-address> 0 0
    ```
    This will send 10 ping requests to the specified IPv6 address with 0 delay and no payload.

## 3. Writing a Custom Shell Command
1.  Run `git checkout -f task_3`

2.  Add a function with the following signature to `main.c`:

    ```c
    int cmd_func(int argc, char **argv);
    ```
    As an example, you can implement this function to print out a certain string by using
    `printf();` or `puts();`. You could also turn the onboard LED on with `LED_ON;` and turn it off
    with `LED_OFF;`. Any parameters following the shell command in the RIOT shell are accessible
    from the `argv` variable. `argc` contains the number of parameters used to call this shell
    command plus one for the name of the command. If you are using `LED_ON;` or `LED_OFF;` then you
    need to include the file `board.h`.

    Your function must return `0` if it runs successfully and `-1` if an error occurs.

3.  Add your function to the `shell_commands` array:
    ```c
    static const shell_command_t shell_commands[] = {
        { "command", "command description", cmd_func },
        { NULL, NULL, NULL }
    };
    ```

4.  Flash the modified code to your board and test your new shell command.
    ```sh
    BOARD=samr21-xpro make all flash term
    ```
    You can verify with `help` if your command was picked up by the shell handler. Call your
    shell command and observe the output.

5.  Run `git checkout -f task_3_solution` to see one of many solutions. Keep in mind that all
    changes that you made to the working directory will be discarded when running this command.

6.  *(Optional)* Have a peek at RIOT's shell commands in `/sys/shell/commands`.

## 4. Adding a CoAP End-Point
1.  Run `git checkout -f task_4`

2.  [CoAP](https://tools.ietf.org/html/rfc7252) is a web transfer protocol with very similar features
    to HTTP. In fact, CoAP was designed to translate easyily to HTTP, but still meets necessary
    requirements to operate with constrained devices in constrained networks. In order to provide
    a new service that can be requested from a CoAP client, we need to take three steps:
    * add a new callback function that is called whenever the service is requested. This callback
      function determines the content of the response.
    * add an `end-point path`. This basically resembles an URI that we need to link to the new
      callback function.
    * add a new `end-point`. This is the structure that links the new callback function and the
      URI together.

3.  In order to add a new CoAP end-point we first need to create a callback function in `main.c`.
    Have a look at the `handle_get_riot_board()` function. It is necessary that your function
    implements the same signature and returns the same values as `handle_get_riot_board()`.
    In your new callback function, modify `response` so that it includes the status of the LED.

4.  Add an `end-point path` that represents your new callback function. You can chain several strings
    together like it was done for `path_riot_board`. These strings will resemble the url that is used
    to call your new callback. For example, `path_riot_board`: `{ "riot", "board" }` becomes `riot/board`.

5.  Add the new `end-point`. Use `COAP_METHOD_GET` as the request type and link to your custom
    callback function and `end-point path`. Use `0` for the
    [content type](https://tools.ietf.org/html/rfc7252#section-12.3) (`ct=0`).

6.  Flash the binary of your solution to the hardware:
    ```sh
    BOARD=samr21-xpro make all flash term
    ```

7.  Approach the instructor and announce your global IPv6 address (not beginning with `fe80`).
    The instructor will then do a CoAP request in order to verify your solution.

8.  Run `git checkout -f task_4_solution` to see one of many solutions. Keep in mind that all
    changes that you made to the working directory will be discarded when running this command.

## 5. Control LED of a Neighbor's Board remoteley
1.  Run `git checkout -f task_5`

2. This task uses posix sockets in order to drive a simple UDP server and client application.
   The advantage of using posix sockets is their portability onto other systems that implement
   the posix API. Have a look at `udp.c` and also at the new shell command declared in `main.c` to
   send a UDP message directly from the shell. Give it a try: send a message to your neighbor's
   IPv6 address with the command `udp <IPv6-address> <msg>`.

3. The current implementation of the UDP server prints out the received message. Change this behavior
   in `udp.c` to check the incoming message and react appropriately. Turn the LED on if you receive
   `on` and turn the LED off if you receive `off`. Do not forget to mirror the changes to the `led_status`
   variable.

4. Test your implementation with the global IPv6 address of your neighbor's node.

5. The instructor can then again verify with a CoAP request that your program successfully
   controlled your neighbor's node remotely.

6.  Run `git checkout -f task_5_solution` to see one of many solutions. Keep in mind that all
    changes that you made to the working directory will be discarded when running this command.
