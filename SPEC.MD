# Psyhic Octo Tribble

## Purpose

This is a research grade, applicaiton who's aim is to demonstrate the differnece between a single-threaded low latency engine, versus a multi-threaded high throughput engine. We will be paying particular attention to

* Cache coherenece
* memory odering
* contention
* fairness degradation under load

---

## Scope

### Within Scope

* Single symbol BTC
* Continuous double auction
* Limit + market orders only
* Deterministic event processing
* Explicit performance and fairness measurements
* Cache-level reasoning (L1/L2/L3 effects)

### Explicitly out of scope

* UI
* Networking (beyond local ingestion)
* Authentication
* Wallets / balances
* Multiple symbols
* Regulatory compliance

---

## Research Quesetions

1. How far can a single threadded engine scale before latency collapses?
2. What exactly breaks when multiple threads touch state?
3. How does cache-line contention affect
   1. Latency
   2. priority
4. What guarantees must be relaxed to achieve higher thorughput

---

## Modes

### low latency (Single Writer)

This will be a single threaded engine, that has the following characteristics

* No lcoks on hot paths
* tight poll loop
* no bathcing
* strict price-time priority

Our hypothesis is the following

* Low p99 latency
* perfect FIFO fariness
* lower max throughput

### high throughput (Multi-Threaded ingestion)

This will be a multi threaded model with the following characteristics

* Multiple producers
* singel or sharded mathcing logic
* controlled batching
* explicit sequence layer
* relaxed latency guranteeds

Our hypothesiss is the follwoing

* High throughput
* increased latency variance
* mesasurable fairness drift

---

## Determinism

Determinism definition

* Given:
  * the same ordered input event stream
  * the same engine configuration
* And
  * Every input event has a sequence number
  * Time is not used to break ties
  * No reliance on thread scheduling for semantics
  * Replay is a first-class requirement
* Then the engine must:
  * produce the same trades in the same order
  * with identical prices and quantities

_Determinism may be explicitly relaxed in throughput mode iff:_

* the relaxation is documented
* the deviation is measurable

---

## Fairness

Both engines will follow the __price-time priority__

_1._

* Given 2 prices of different levels
* Then the better price always matches first

_2._

* Given 2 prices of tghe same level
* Then the earlier price matches first

_3._

* Given a reordering within a price level is documented
* Then reordering is allowed

### Note

_The following fairness violations are not bugs_

* Only occur in throughput mode
* Are measured and explained

---

## Order lifecycle

Every order will go through the following lifecycle

```json
NEW -> ACCEPTED (PARTIAL_FILL) -> [FILLED|CANCELLED|REJECTED]
```

The follwoing rules apply

* Matching is atomic
* Partial fills emit events
* Rejections are deterministic
* No observert see intermediate state

---

## Performance Metrics

### Engine metrics

* Trade/sec
* Orders/sec
* Median/ p99/ p999 latency
* Cache misses (stretch metric)

### Engine comparison metrics

* Batched vs non batched
* fariness drift (under contention)
