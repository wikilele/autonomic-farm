# Autonomic Farm Framework

Parallel and Distributed Systems final project

## Usage

Building the C++ native thread implementation

```
    git clone https://github.com/wikilele/autonomic-farm.git
    cd autonomic-farm

    chmod +x scripts/build.sh
    chmod +x scripts/run.sh
    chmod +x scripts/plot.sh

    ./scripts/build.sh
    ./scripts/run.sh

    # to plot graph-cli is needed
    head plot.sh
     ./scripts/plot.sh
```

Building the fast-flow implementation
```
    # must put fast flow under the lib directory
    # if we want to build also the fastflow part
    cd lib
    git clone https://github.com/fastflow/fastflow.git
    cd ..

    chmod +x scripts/ffbuild.sh
    chmod +x scripts/ffrun.sh

    ./scripts/ffbuild.sh
    ./scripts/ffrun.sh


    
```