Nanolat Database
================

What is it?
-----------
- An in-memory key/value store supporting put/get/del, and cursors for range search.
- Supports transaction and master-slave replication.
- Supports Table Plugins to implement your own data structure for efficient data processing for your specific domain.

Target systems
--------------
- Financial systems such as HFT(High Frequency Trading), FEP(Front End Processor), etc.
- Monitoring systems that require processing huge amount of data in a second.

Why use it?
-----------
- Low latency data processing, 1 transaction takes < 5 us.
- Supports 1.5M transactions / second .
- Supports built in Table Plug-ins using google LevelDB and TokyoCabinet providing replication feature on top of them. 
- (Under Development in another project) Supports basic SQL that is directly converted to Nanolat Database API at compile time to achieve low latency data processing.

How it works?
-------------
- Uses 40G Infiniband and 10G Ethernet instead of TCP/IP to replicate data from a server to another server within 2.5 us.
- Instead of flushing transactional log on local disk, transfer it to slave machine.

Nanolat License v1.0
--------------------
- In the hope of seeing increasing contributions in countries that does not have enough open source contributors,
  Nanolat License provides free LGPL v2.1 only to companies in countries in top 10 open source contributing countries.
- For companies in other countries, you are under GPL v3.0, but you still can buy LGPL v2.1 from Nanolat Co., Ltd.
- However, for non-commercial usage, you get free LGPL v2.1 whereever you are.
