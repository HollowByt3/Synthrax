**Synthrax**

Synthrax AI is a next-generation, AI-driven SIEM that eliminates alert fatigue and dramatically improves threat detection accuracy using confidential vector intelligence powered by CyborgDB. Traditional SIEMs overwhelm teams with false positives, massive data costs, and outdated rule-based detection. Synthrax flips the model: it ingests logs, converts them into encrypted vector embeddings, and performs threat analysis inside CyborgDB meaning detection happens while data remains encrypted at rest, in transit, and even in use. This enables privacy preserving anomaly detection, secure user behavior analytics, and cross-environment threat correlation without exposing sensitive logs.

**Prerequisites**
before you continue to installation please make sure you have the following prerequisites installed:
Cmake
rsyslog with Omprog module

**Get Started**

run the following commands:

mkdir build && cd build
cmake ..
sudo mv 60-synthrax-gateway.conf /etc/rsyslog.d/60-synthrax-gateway.conf

