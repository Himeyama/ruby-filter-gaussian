

```sh
mkdir -p /usr/local/share
git clone --depth 1 https://github.com/ruby-numo/numo-narray.git /tmp/numo-narray
cd /tmp/numo-narray/ext/numo/narray
ruby extconf.rb
make -j
sudo mkdir -p /usr/local/include
sudo cp narray.so /usr/lib/libnarray.so
sudo cp -r numo /usr/local/include/
cd -

```

```sh
sudo rm -rf /usr/local/include/numo/
sudo rm /usr/lib/libnarray.so
rm -rf /tmp/numo-narray/
```