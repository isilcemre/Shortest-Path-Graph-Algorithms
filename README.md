# Pathfinding Algorithms Benchmark (BFS, Dijkstra, A*)

Bu proje, büyük ölçekli coğrafi veri setleri üzerinde en kısa yolu bulmak için kullanılan üç temel algoritmanın (**Breadth-First Search**, **Dijkstra** ve **A***) C dili ile gerçekleştirilmiş performans karşılaştırmasını içerir.

## Algoritmalara Genel Bakış

Proje kapsamında aşağıdaki algoritmaların çalışma mantıkları ve performansları analiz edilmektedir:

* **BFS (Breadth-First Search):** Ağırlıksız grafiklerde en az düğüm sayısına sahip yolu bulur. Bu projede ağırlıklı grafik yapısı kullanıldığı için "adım sayısı" odaklı çalışır.
* **Dijkstra:** Kenar ağırlıklarını (mesafe) dikkate alarak başlangıç noktasından hedefe olan en kesin kısa yolu bulur.
* **A* (A-Star):** Dijkstra algoritmasının üzerine "sezgisel" (heuristic) bir fonksiyon ekleyerek hedef yönüne doğru aramayı optimize eder. Bu projede sezgisel olarak **Öklid Mesafesi** kullanılmıştır.

## Teknik Detaylar

* **Dil:** C
* **Veri Yapısı:** Komşuluk Listesi (Adjacency List)
* **Optimizasyon:** Dijkstra ve A* için **Priority Queue (Öncelik Kuyruğu)** yapısı kullanılmıştır.
* **Hız Ölçümü:** Her algoritma için `clock()` fonksiyonu kullanılarak CPU çalışma süresi hesaplanmıştır.

## Dosya İçerikleri

* `main.c`: Grafik yapısının oluşturulması, algoritmaların implementasyonu ve G/Ç işlemlerini içeren kaynak kod.
* `in1000.txt`: 1.000 düğüm ve 11.343 kenar içeren orta ölçekli test verisi.
* `in100000.txt`: 100.000 düğüm ve 1.158.612 kenar içeren büyük ölçekli test verisi.

##  Başlangıç

### Derleme
GCC derleyicisi kullanarak projeyi aşağıdaki komutla derleyebilirsiniz:
```bash gcc main.c -o pathfinder -lm```

### Notlar
* Büyük veri setlerinde (in100000.txt) A* algoritmasının Dijkstra'ya göre ne kadar daha az düğüm ziyaret ederek hedefe ulaştığını CPU sürelerinden gözlemleyebilirsiniz.
