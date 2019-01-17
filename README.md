# mchat-core

This was going to be a c++ http api server but after a few benchmarks it doesn't really make a difference in performance. I believe the bottleneck is the network. There should be no issues scaling since in my benchmarks we were using minimal resources on the server, and only bottlenecked by the hardware.

So, this means we are going back to python. I will keep this repository up becuase I'd still like to work on my c++ skills but it will not be serving mchat requests for quite some time (if ever). 
