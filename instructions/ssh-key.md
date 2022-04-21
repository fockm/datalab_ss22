# Generating a ssh key

Open a terminal or command line and execute the following command:

```
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```
OR

```
ssh-keygen -t ed25519 -C "your_email@example.com"
```

depending on what algorithm you want to use (`ed25519` is the newer one). Then follow the command prompt and either keed the default path `$HOME/.ssh/id_rsa` or give the file a new name, but remember the full path to this file! (`$HOME/.ssh/id_rsa_new_name`)

> Run `ssh-keygen --help` or `man ssh-keygen` if you want to see what the options mean and how to use the executable. For example `[-t dsa | ecdsa | ed25519 | rsa]`: the `-t` option is the type of algorithm to use and allows one of the 4 options before (`-b`is the byte size and `-C` is an optional comment)

> If you decide to use the `ed25519` algorithm you will need to perform an additional step of adding the _'ssh-key'_ to your _'keychain'_; depending on your system this might not be done automatically as opposed to the `sha` keys (more info [here](https://gist.github.com/hrdtbs/ba50868d7d608b89f958fe32dc35fdd4)). Both algorithms are secure and the speed difference is no issue when generating just one key for one user (more discussions on this topic [here](https://security.stackexchange.com/questions/90077/ssh-key-ed25519-vs-rsa))
```
ssh-add -K ~/.ssh/id_ed25519
```

Once you have the new key generated (and active) add it to your Github account by copying the contents of the __public__ key to the clipboard.
For this you can either open `~/.ssh/id_<keyname>.pub`and copy the contents or use a command line such as `cat`pr `pbcopy`or `xclip`if you have them installed:

```
cat ~/.ssh/id_rsa
```

Copy the contents (that are displayed in the terminal), then go to [github](https://gihub.com), log in go to your account settings

![Screenshot 2021-10-14 at 12 47 42](https://user-images.githubusercontent.com/6952640/137303435-3b98e613-edbf-41f4-94c2-e7a3542b2c7b.png)

then select _SSH and GPG keys_>_New SSH key_, paste the contents and save the new key! 
