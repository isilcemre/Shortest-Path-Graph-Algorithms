# Pathfinding Algorithms Benchmark (BFS, Dijkstra, A*)

# Pathfinding Algorithms Benchmark (BFS, Dijkstra, A*)

This project features a performance comparison of three fundamental algorithms used to find the shortest path on large-scale geographical datasets—**Breadth-First Search**, **Dijkstra**, and **A***—implemented in C.

## Algorithms Overview

The logic and performance of the following algorithms are analyzed within the scope of this project:

* **BFS (Breadth-First Search):** Finds the path with the minimum number of nodes in unweighted graphs. Since this project utilizes a weighted graph structure, it operates with a focus on "step count."
* **Dijkstra:** Finds the most accurate shortest path from the starting point to the destination by considering edge weights (distance).
* **A* (A-Star):** Optimizes the search toward the destination by adding a "heuristic" function to the Dijkstra algorithm. In this project, **Euclidean Distance** is used as the heuristic.

## Technical Details

* **Language:** C
* **Data Structure:** Adjacency List
* **Optimization:** A **Priority Queue** structure is used for both Dijkstra and A*.
* **Speed Measurement:** CPU execution time is calculated for each algorithm using the `clock()` function.

## File Contents

* `main.c`: Source code containing graph structure creation, algorithm implementation, and I/O operations.
* `in1000.txt`: Medium-scale test data containing 1,000 nodes and 11,343 edges.
* `in100000.txt`: Large-scale test data containing 100,000 nodes and 1,158,612 edges.

### Compilation
You can compile the project using the GCC compiler with the following command:
```bash
gcc main.c -o pathfinder -lm bash
```

# Yol Bulma Algoritmaları Performans Karşılaştırması (BFS, Dijkstra, A)*

Bu proje, büyük ölçekli coğrafi veri setleri üzerinde en kısa yolu bulmak için kullanılan üç temel algoritmanın (**Breadth-First Search**, **Dijkstra** ve **A***) C dili ile gerçekleştirilmiş performans karşılaştırmasını içerir.

## Algoritmalara Genel Bakış

Proje kapsamında aşağıdaki algoritmaların çalışma mantıkları ve performansları analiz edilmektedir:

* **BFS (Breadth-First Search):** Ağırlıksız grafiklerde en az düğüm sayısına sahip yolu bulur. Bu projede ağırlıklı grafik yapısı kullanıldığı için "adım sayısı" odaklı çalışır.
* **Dijkstra:** Kenar ağırlıklarını (mesafe) dikkate alarak başlangıç noktasından hedefe olan en kesin kısa yolu bulur.
* **A'*' (A-Star):** Dijkstra algoritmasının üzerine "sezgisel" (heuristic) bir fonksiyon ekleyerek hedef yönüne doğru aramayı optimize eder. Bu projede sezgisel olarak **Öklid Mesafesi** kullanılmıştır.

## Teknik Detaylar

* **Dil:** C
* **Veri Yapısı:** Komşuluk Listesi (Adjacency List)
* **Optimizasyon:** Dijkstra ve A* için **Priority Queue (Öncelik Kuyruğu)** yapısı kullanılmıştır.
* **Hız Ölçümü:** Her algoritma için `clock()` fonksiyonu kullanılarak CPU çalışma süresi hesaplanmıştır.

## Dosya İçerikleri

* `main.c`: Grafik yapısının oluşturulması, algoritmaların implementasyonu ve G/Ç işlemlerini içeren kaynak kod.
* `in1000.txt`: 1.000 düğüm ve 11.343 kenar içeren orta ölçekli test verisi.
* `in100000.txt`: 100.000 düğüm ve 1.158.612 kenar içeren büyük ölçekli test verisi.


### Derleme
GCC derleyicisi kullanarak projeyi aşağıdaki komutla derleyebilirsiniz:
```bash gcc main.c -o pathfinder -lm```

### Notlar
* Büyük veri setlerinde (in100000.txt) A* algoritmasının Dijkstra'ya göre ne kadar daha az düğüm ziyaret ederek hedefe ulaştığını CPU sürelerinden gözlemleyebilirsiniz.
