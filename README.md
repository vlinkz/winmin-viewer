# Winmin Viewer

Winmin Viewer is an application that uses the [SPICE protocol](https://www.spice-space.org/) to display applications from [libvirt](https://libvirt.org/) virtual machines.

# Build Dependencies

- libspice-client-gtk-3.0-dev
- libspice-client-glib-2.0-dev
- libgtk-3-dev
- meson

# Building

Build and install with:

```
$ meson build
$ ninja -C build install
```
# Runtime Dependencies

- spice-client-gtk

# Running

In order for this program to access the spice socket of the guest VM, the user must be part of the `kvm` group. This can be added using the following command.
```
$ sudo usermod -aG $USER kvm
```

You may need to logout or reboot in order for group changes to take effect.

### Usage
```
winmin-viewer name spice_socket
```
### Example
```
winmin-viewer "Word" spice+unix:///var/lib/libvirt/qemu/domain-1-winmin-msoffice/spice.sock
```
# TODO
- Format commandline arguments better see: [Parsing command-line arguments in C?](https://stackoverflow.com/questions/9642732/parsing-command-line-arguments-in-c)
- Add fullscreen option
- Add direct binding to libvirt virtual machines using the libvirt API
- Add USB passthrough options