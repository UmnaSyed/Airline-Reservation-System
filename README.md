# Airline Reservation System

A console-based Airline Reservation System written in **C++**, built around core data structures and algorithms — a Binary Search Tree for flight storage, a priority heap for waitlist management, a graph with Dijkstra's algorithm for route-finding, and a hash table for airport lookups.

## Overview

This project simulates a real-world airline booking system: adding and managing flights, reserving and cancelling seats, handling overbooked flights via a priority waitlist, and finding the cheapest route between airports across a network of connecting flights. All data persists to disk between runs.

## Features

- **Add / List / Search / Delete flights** — flights are stored and indexed by Flight ID
- **Reserve & cancel seats** — with automatic promotion from the waitlist when a seat opens up
- **Priority-based waitlisting** — passengers are waitlisted by class (First / Business / Economy) and booking order when a flight is full
- **Cheapest route finder** — computes the lowest-cost path between two airports across the flight network, even across connecting flights
- **Sort flights by price**
- **Round-trip booking**
- **Runtime benchmarking** — built-in comparison of search and sorting algorithm performance on the current flight data
- **Persistent storage** — flights, waitlists, and passenger history are saved to and loaded from disk automatically

## 🏗️ Data Structures & Algorithms

| Component | Structure / Algorithm | Purpose |
|---|---|---|
| **Flight storage** | Binary Search Tree (BST), keyed by Flight ID | Fast insert, lookup, and deletion of flights |
| **Waitlist** | Custom binary max-heap (priority queue) | Orders waitlisted passengers by class priority, then booking time |
| **Route network** | Graph (adjacency list) + Dijkstra's algorithm | Finds the cheapest route between airports |
| **Airport lookup** | Hash table (open addressing) | Maps airport names to graph indices |
| **Sorting** | Quick Sort, Selection Sort, Bubble Sort | Available for sorting flights by price; also benchmarked against each other |
| **Persistence** | File I/O (`flights.txt`, `waitlists.txt`, `passenger_history.txt`) | Saves and restores system state between sessions |

## 📋 Menu Options

```
 1.  Add Flight
 2.  List Flights
 3.  Reserve Seat
 4.  Cancel Seat
 5.  Find Cheapest Route
 6.  Display Waitlist
 7.  Search Flights
 8.  Sort Flights by Price
 9.  Delete Flight
 10. Manage Waitlist
 11. Round-trip Booking
 12. Test Runtimes
 13. Exit
```

## Booking Priority

When reserving a seat, passengers select a travel class, which determines their priority if the flight is full and they're placed on the waitlist:

| Class | Priority |
|---|---|
| First | 3 (highest) |
| Business | 2 |
| Economy | 1 |

Waitlisted passengers are promoted automatically, highest priority first, whenever a seat is cancelled.

## Files

| File | Description |
|---|---|
| `main.cpp` | Full source code — BST, heap, graph, hash table, and menu-driven interface |
| `flights.txt` | Persisted flight records (auto-generated/updated at runtime) |
| `waitlists.txt` | Persisted waitlist entries per flight (auto-generated at runtime) |
| `passenger_history.txt` | Log of booking/cancellation actions (auto-generated at runtime) |

## Getting Started

### Compile

```bash
g++ -std=c++11 -o airline_system main.cpp
```

### Run

```bash
./airline_system
```

On startup, the system automatically loads any existing flights and waitlists from `flights.txt` and `waitlists.txt`.

## Runtime Testing

Option **12 (Test Runtimes)** benchmarks, on the current flight data:
- BST search vs. linear search
- Quick Sort vs. Selection Sort vs. Bubble Sort
- Heap push operations
- Dijkstra's shortest path computation

This makes it easy to see the practical performance difference between the data structures/algorithms used.

## Possible Improvements

- Input validation hardening (e.g., duplicate/invalid airport codes)
- A graphical or web-based front end
- Support for multi-leg itineraries beyond round-trips
- Database backend instead of flat-file persistence

## License

This project was developed for academic/learning purposes as a data structures and algorithms exercise.
