MARKET DATA → ORDER BOOK → ANALYSIS → KAFKA → STORY SERVICE
PROJECT PLAN (C++ PERFORMANCE FOCUSED)

PHASE 0 — GLOBAL CONSTRAINTS AND PRINCIPLES

GOAL
Define non-negotiable rules so the system stays performance-driven and realistic.

CONSTRAINTS

Single C++ process for core engine

Zero heap allocation after initialization

No mutexes on hot path

Single-writer per symbol

Multi-reader without blocking

Cache-line-aware data layout

Explicit std::atomic memory orderings

Latency measured using TSC (rdtsc / rdtscp)

Kafka is strictly outside the hot path

TECHNICAL NOTES

All memory allocated upfront (arena / slabs)

No std::string, no virtual dispatch in hot path

std::atomic defaults are forbidden; must specify ordering

PHASE 1 — MARKET DATA INGEST

GOAL
Receive and normalize market data at high throughput with predictable latency.

SCOPE

Simulated binary market data feed (initially)

Fixed-size messages

Single instrument to start

ARCHITECTURE
NIC / socket
→ RX thread (busy poll)
→ lock-free ring buffer
→ parser / normalizer

TECHNICAL DETAILS

UDP socket (localhost initially)

Busy polling (no epoll/select)

One RX thread pinned to a core

Lock-free SPSC ring buffer

Messages are POD structs (no pointers)

DELIVERABLE

Sustain ≥1M messages/sec locally

Track packet drops and sequence gaps

Stable p99 latency under load

PHASE 2 — ORDER BOOK CORE

GOAL
Maintain a correct, low-latency limit order book.

SCOPE (STRICT)

One symbol

Aggregated price levels only

Add + trade messages only (no cancels yet)

DATA MODEL

Fixed price ladder

Array-indexed price levels

Structure-of-arrays (SoA) layout

CONCURRENCY MODEL

Single writer thread owns the book

No reader ever mutates book state

Writer never blocks

TECHNICAL DETAILS

Price → index mapping via offset math

Cache-line alignment for hot fields

No std::map / unordered_map

All book state fits in L2 if possible

DELIVERABLE

Correct best bid / best ask

Deterministic updates

Zero locks, zero allocations

PHASE 3 — SNAPSHOT & READ MODEL

GOAL
Expose order book state to readers without slowing the writer.

DESIGN

Double-buffered snapshots

Writer publishes snapshot pointer

Readers load snapshot pointer atomically

MEMORY MODEL

Writer: store-release

Reader: load-acquire

SNAPSHOT CONTENT

Best bid price + size

Best ask price + size

Spread

Order book imbalance

Timestamp (TSC)

TECHNICAL DETAILS

Snapshot structs are immutable after publish

No per-reader copies

Readers can sample at arbitrary rates

DELIVERABLE

Readers always see consistent state

Writer throughput unchanged

Verified correctness under stress

PHASE 4 — ANALYSIS LAYER (C++)

GOAL
Extract meaningful signals without contaminating the hot path.

RULES

Analysis consumes snapshots only

No shared mutable state with book

Bounded queues only

EXAMPLE ANALYSES (PICK 2–3)

Spread widening detection

Liquidity imbalance spikes

Microprice shifts

Aggressive trade detection

TECHNICAL DETAILS

Dedicated analysis thread(s)

Backpressure-aware queues

Deterministic behavior under replay

DELIVERABLE

Signal events with timestamps

Measured CPU cost

No impact on book latency

PHASE 5 — KAFKA PUBLICATION

GOAL
Export aggregated insights safely to external systems.

WHAT GOES TO KAFKA

Signal events

Periodic summaries (e.g. every 100ms)

WHAT DOES NOT

Raw order book updates

Per-message feed data

TECHNICAL DETAILS

Kafka producer runs outside hot path

Bounded internal buffers

Drop or coalesce on backpressure

Engine must continue if Kafka is down

DELIVERABLE

Stable event schema

No cascading failures

Measured publication overhead

PHASE 6 — STORY / VALUE SERVICE (SEPARATE SYSTEM)

GOAL
Turn technical signals into human-readable insights.

TECH STACK

Any language (performance irrelevant)

Kafka consumer

Database for persistence

RESPONSIBILITIES

Consume Kafka events

Store time-series data

Generate narratives like:

“Liquidity vanished on bid”

“Aggressive buying detected”

“Spread instability event”

KEY RULE

This service can crash or be killed without affecting the C++ engine

DELIVERABLE

User-facing insights

Queryable history

Zero coupling to core engine

PHASE 7 — MEASUREMENT & VALIDATION

GOAL
Prove correctness and performance, not assume it.

METRICS

Throughput (msgs/sec)

Latency p50 / p99 / p999

Cache misses (perf)

CPU utilization per thread

TOOLS

perf

cachegrind

custom TSC-based timers

DELIVERABLE

Reproducible benchmarks

Documented performance tradeoffs

END STATE

RESULT

A realistic HFT-style market data and order book engine

Clear separation between hot path and value extraction

Demonstrable mastery of C++ concurrency and architecture
