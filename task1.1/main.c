#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

// Coğrafi koordinatları tanımlamak için yapı
typedef struct {
    double lat; // Enlem
    double lon; // Boylam
} CityCoords;

// Bir kenarı (yolu) temsil eden yapı
typedef struct Edge {
    int to;          // Hedef şehir indeksi
    double distance; // Yolun mesafesi
    struct Edge *next; // Bağlı liste için sonraki kenar
} Edge;

// Grafik yapısı
typedef struct {
    int n_cities;     // Şehir sayısı
    int n_roads;      // Yol sayısı
    CityCoords *coords; // Şehir koordinatlarının dizisi
    Edge **adj;       // Komşuluk listesi (dinamik dizi)
} Graph;

// Global değişkenler
Graph graph;
int start_city, end_city;

// --- Yardımcı Fonksiyonlar ---

// Yeni bir kenar (Edge) düğümü oluşturur
Edge* create_edge(int to, double distance) {
    Edge *new_edge = (Edge*)malloc(sizeof(Edge));
    if (new_edge == NULL) {
        perror("Bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }
    new_edge->to = to;
    new_edge->distance = distance;
    new_edge->next = NULL;
    return new_edge;
}

// Komşuluk listesine kenar ekler (yönlendirilmemiş grafik)
void add_edge(Graph *g, int u, int v, double w) {
    // u -> v kenarı
    Edge *new_uv = create_edge(v, w);
    new_uv->next = g->adj[u];
    g->adj[u] = new_uv;

    // v -> u kenarı (yönlendirilmemiş olduğu için)
    Edge *new_vu = create_edge(u, w);
    new_vu->next = g->adj[v];
    g->adj[v] = new_vu;
}

// Haversine formülü: İki koordinat arasındaki coğrafi mesafeyi hesaplar
// Basitleştirilmiş: Sadece bir sezgisel (hheuristic) için kullanılır
double haversine_distance(CityCoords c1, CityCoords c2) {
    // Dünya yarıçapı (ortalama, km)
    const double R = 6371.0;

    // Dereceleri radyan cinsine çevir
    double lat1_rad = c1.lat * (M_PI / 180.0);
    double lat2_rad = c2.lat * (M_PI / 180.0);
    double dlat_rad = (c2.lat - c1.lat) * (M_PI / 180.0);
    double dlon_rad = (c2.lon - c1.lon) * (M_PI / 180.0);

    // Haversine formülü
    double a = sin(dlat_rad / 2.0) * sin(dlat_rad / 2.0) +
               cos(lat1_rad) * cos(lat2_rad) *
               sin(dlon_rad / 2.0) * sin(dlon_rad / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return R * c; // Hava yolu mesafesi (sezgisel için)
}

// Grafiğin belleğini serbest bırakır
void free_graph(Graph *g) {
    if (g->coords != NULL) {
        free(g->coords);
    }
    if (g->adj != NULL) {
        for (int i = 1; i <= g->n_cities; i++) {
            Edge *current = g->adj[i];
            while (current != NULL) {
                Edge *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(g->adj);
    }
}

// --- Girdi Okuma Fonksiyonu ---

void read_input() {
    // n ve m değerlerini oku
    if (scanf("%d %d", &graph.n_cities, &graph.n_roads) != 2) {
        fprintf(stderr, "Hata: n ve m okunamadi.\n");
        exit(EXIT_FAILURE);
    }

    // Koordinat ve komşuluk listesi için bellek tahsis et
    // Şehirler 1'den n'ye numaralandığı için, 0. indeksi boş bırakıyoruz (n+1 boyutunda)
    graph.coords = (CityCoords*)malloc((graph.n_cities + 1) * sizeof(CityCoords));
    graph.adj = (Edge**)calloc(graph.n_cities + 1, sizeof(Edge*));

    if (graph.coords == NULL || graph.adj == NULL) {
        perror("Bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }

    // Şehir koordinatlarını oku
    for (int i = 1; i <= graph.n_cities; i++) {
        if (scanf("%lf %lf", &graph.coords[i].lat, &graph.coords[i].lon) != 2) {
            fprintf(stderr, "Hata: Koordinatlar okunamadi.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Yol (kenar) bilgilerini oku
    for (int i = 0; i < graph.n_roads; i++) {
        int v1, v2;
        double w;
        if (scanf("%d %d %lf", &v1, &v2, &w) != 3) {
            fprintf(stderr, "Hata: Yol bilgileri okunamadi.\n");
            exit(EXIT_FAILURE);
        }
        add_edge(&graph, v1, v2, w);
    }

    // Başlangıç ve bitiş şehirlerini oku
    if (scanf("%d %d", &start_city, &end_city) != 2) {
        fprintf(stderr, "Hata: Baslangic ve bitis sehirleri okunamadi.\n");
        exit(EXIT_FAILURE);
    }
}

// Bulunan yolu yazdıran yardımcı fonksiyon
void print_path(int *parent, int end_node) {
    // Yolun düğümlerini tutacak dinamik dizi (stack olarak kullanacağız)
    int *path = (int*)malloc((graph.n_cities + 1) * sizeof(int));
    if (path == NULL) {
        perror("Bellek tahsisi hatasi");
        return;
    }
    int path_len = 0;
    int curr = end_node;

    // Bitişten başlayarak parent dizisi ile geriye doğru git ve yolu oluştur
    while (curr != 0) { // BFS ve Dijkstra/A* için parent[start_city] = 0
        path[path_len++] = curr;
        curr = parent[curr];
    }

    // Yolu tersten yazdır (başlangıçtan bitişe)
    for (int i = path_len - 1; i >= 0; i--) {
        printf("%d%s", path[i], (i == 0) ? "" : " ");
    }
    printf("\n");

    free(path);
}

// --- Kuyruk (Queue) Yapısı: BFS için ---

typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;// bas
    QueueNode *rear; //son
} Queue;

// Kuyruğu başlatır
void init_queue(Queue *q) {
    q->front = q->rear = NULL;
}

// Kuyruğa eleman ekler
void enqueue(Queue *q, int data) {
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        perror("Kuyruk bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

// Kuyruktan eleman çıkarır
int dequeue(Queue *q) {
    if (q->front == NULL) {
        return -1; // Hata veya boş kuyruk
    }
    QueueNode *temp = q->front;
    int data = temp->data;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

// Kuyruk boş mu kontrol eder
int is_empty(Queue *q) {
    return q->front == NULL;
}

// Kuyruğu serbest bırakır
void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
}

// --- Öncelik Kuyruğu (Priority Queue) Yapısı: Dijkstra ve A* için ---

// Öncelik Kuyruğu Düğümü
typedef struct PQNode {
    int city;         // Şehir indeksi
    double priority;  // Öncelik değeri (Dijkstra'da mesafe, A*'da f=g+h)
    struct PQNode *next;
} PQNode;

// Öncelik Kuyruğu
typedef struct {
    PQNode *head;
} PriorityQueue;

// Öncelik Kuyruğunu başlatır
void init_pq(PriorityQueue *pq) {
    pq->head = NULL;
}

// Öncelik Kuyruğuna eleman ekler (küçük öncelik daha yüksek)
void pq_push(PriorityQueue *pq, int city, double priority) {
    PQNode *new_node = (PQNode*)malloc(sizeof(PQNode));
    if (new_node == NULL) {
        perror("PQ bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }
    new_node->city = city;
    new_node->priority = priority;
    new_node->next = NULL;

    // Başlangıçta veya baştan daha küçük öncelik
    if (pq->head == NULL || priority < pq->head->priority) {
        new_node->next = pq->head;
        pq->head = new_node;
    } else {
        // Doğru sırayı bul
        PQNode *current = pq->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

// Öncelik Kuyruğundan en yüksek öncelikli elemanı çıkarır
int pq_pop(PriorityQueue *pq, double *priority) {
    if (pq->head == NULL) {
        return -1; // Boş
    }
    PQNode *temp = pq->head;
    int city = temp->city;
    *priority = temp->priority;
    pq->head = pq->head->next;
    free(temp);
    return city;
}

// Öncelik Kuyruğu boş mu
int pq_is_empty(PriorityQueue *pq) {
    return pq->head == NULL;
}

// Öncelik Kuyruğunu serbest bırakır
void free_pq(PriorityQueue *pq) {
    while (!pq_is_empty(pq)) {
        double dummy_priority;
        pq_pop(pq, &dummy_priority);
    }
}

// --- Algoritmalar ---

/**
 * 1) BFS (Breadth-First Search) Algoritması
 * En az sayıda şehir üzerinden geçen yolu bulur (yani en az sayıda kenar).
 * Mesafe/ağırlık dikkate alınmaz.
 */
void bfs_algorithm() {

    // Çalışma süresi ölçümü için başlangıç zamanı
    clock_t start_time = clock();

    // parent dizisi: yolu yeniden oluşturmak için (Örn: parent[v] = u, u'dan v'ye gelindi)
    // 0: başlangıç, -1: ziyaret edilmedi
    int *parent = (int*)calloc(graph.n_cities + 1, sizeof(int));
    // distance dizisi: başlangıçtan o şehire olan kenar sayısını tutar
    int *dist = (int*)malloc((graph.n_cities + 1) * sizeof(int));
    if (parent == NULL || dist == NULL) {
        perror("BFS bellek tahsisi hatasi");
        free(parent);
        free(dist);
        return;
    }

    // Başlangıç durumu
    for (int i = 1; i <= graph.n_cities; i++) {
        dist[i] = -1; // Ziyaret edilmedi
    }
    dist[start_city] = 0;
    parent[start_city] = 0; // Başlangıç düğümünün parent'ı 0 (kendinden önceki yok)

    Queue q;
    init_queue(&q);
    enqueue(&q, start_city);

    int path_found = 0;

    // Ana BFS döngüsü
    while (!is_empty(&q)) {
        int u = dequeue(&q); // Kuyruktan bir düğüm çıkar

        // Bitiş şehrine ulaşıldıysa dur
        if (u == end_city) {
            path_found = 1;
            break;
        }

        // u'nun komşularını gez
        Edge *edge = graph.adj[u];
        while (edge != NULL) {
            int v = edge->to;

            // Eğer v ziyaret edilmediyse
            if (dist[v] == -1) {
                dist[v] = dist[u] +1; // Yol uzunluğu +1
                parent[v] = u;         // u, v'nin parent'ı olur
                enqueue(&q, v);        // v'yi kuyruğa ekle
            }
            edge = edge->next;
        }
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Çıktı
    if (path_found) {
        printf("%.0f\n", ((double)dist[end_city])-1); // Path uzunluğu (kenar sayısı)
        print_path(parent, end_city);             // Path üzerindeki düğümler
    } else {
        printf("-1\n-1\n"); // Yol bulunamadı
    }

    printf("BFS Sure: %.6f s\n", time_spent);


    // Belleği serbest bırak
    free_queue(&q);
    free(parent);
    free(dist);
    printf("\n");
}

/**
 * 2) Dijkstra Algoritması
 * Başlangıç ve bitiş şehirleri arasındaki en kısa toplam mesafeyi (ağırlık) bulur.
 */
void dijkstra_algorithm() {
    // Çalışma süresi ölçümü için başlangıç zamanı
    clock_t start_time = clock();

    // dist dizisi: başlangıçtan o şehire olan en kısa mesafeyi tutar
    // double tipinde çünkü mesafeler double
    double *dist = (double*)malloc((graph.n_cities + 1) * sizeof(double));
    // parent dizisi: yolu yeniden oluşturmak için
    int *parent = (int*)calloc(graph.n_cities + 1, sizeof(int));
    if (dist == NULL || parent == NULL) {
        perror("Dijkstra bellek tahsisi hatasi");
        free(dist);
        free(parent);
        return;
    }

    // Başlangıç durumu
    for (int i = 1; i <= graph.n_cities; i++) {
        dist[i] = HUGE_VAL; // Sonsuz olarak ayarla
    }
    dist[start_city] = 0.0;
    parent[start_city] = 0;

    PriorityQueue pq;
    init_pq(&pq);
    // (şehir, öncelik) -> (şehir, mesafe)
    pq_push(&pq, start_city, 0.0);

    int path_found = 0;

    // Ana Dijkstra döngüsü
    while (!pq_is_empty(&pq)) {
        double current_dist;
        int u = pq_pop(&pq, &current_dist); // En kısa mesafeli düğümü al

        // Eğer kuyruktan çıkan mesafe, zaten bilinen en kısa mesafeden büyükse, ihmal et (eskimiş düğüm)
        if (current_dist > dist[u]) {
            continue;
        }

        // Bitiş şehrine ulaşıldıysa
        if (u == end_city) {
            path_found = 1;
            break;
        }

        // u'nun komşularını gez
        Edge *edge = graph.adj[u];
        while (edge != NULL) {
            int v = edge->to;
            double weight = edge->distance;

            // Relaksasyon: Daha kısa bir yol bulunduysa
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                // Yeni mesafeyle öncelik kuyruğuna ekle
                pq_push(&pq, v, dist[v]);
            }
            edge = edge->next;
        }
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;


    // Çıktı
    if (path_found) {
        printf("%.2f\n", dist[end_city]); // Path uzunluğu (toplam mesafe)
        print_path(parent, end_city);     // Path üzerindeki düğümler
    } else {
        printf("-1.00\n-1\n"); // Yol bulunamadı
    }
    printf("Dijkstra Sure: %.6f s\n", time_spent);

    // Belleği serbest bırak
    free_pq(&pq);
    free(parent);
    free(dist);
    printf("\n");
}

/**
 * 3) A* (A-star) Algoritması
 * En kısa toplam mesafeyi (ağırlık) bulur, coğrafi sezgisel (hava yolu mesafesi) kullanır.
 * Öncelik: f(n) = g(n) + h(n), burada g(n) başlangıçtan n'ye olan gerçek maliyet, h(n) n'den bitişe olan tahmini maliyettir.
 */
void astar_algorithm() {

    // Çalışma süresi ölçümü için başlangıç zamanı
    clock_t start_time = clock();

    // g_score: başlangıçtan o şehire olan gerçek en kısa mesafeyi (g(n)) tutar
    double *g_score = (double*)malloc((graph.n_cities + 1) * sizeof(double));
    // parent dizisi: yolu yeniden oluşturmak için
    int *parent = (int*)calloc(graph.n_cities + 1, sizeof(int));
    if (g_score == NULL || parent == NULL) {
        perror("A* bellek tahsisi hatasi");
        free(g_score);
        free(parent);
        return;
    }

    // Başlangıç durumu
    for (int i = 1; i <= graph.n_cities; i++) {
        g_score[i] = HUGE_VAL; // Sonsuz olarak ayarla
    }
    g_score[start_city] = 0.0;
    parent[start_city] = 0;

    // Sezgisel (heuristic) maliyet: bitişe olan hava yolu mesafesi h(n)
    // A* için f(n) = g_score[n] + h(n)
    double h_start = haversine_distance(graph.coords[start_city], graph.coords[end_city]);

    PriorityQueue pq;
    init_pq(&pq);
    // (şehir, öncelik) -> (şehir, f(n) = g(n) + h(n))
    pq_push(&pq, start_city, g_score[start_city] + h_start);

    int path_found = 0;

    // Ana A* döngüsü
    while (!pq_is_empty(&pq)) {
        double current_f;
        int u = pq_pop(&pq, &current_f); // En küçük f(n) değerine sahip düğümü al

        // Eğer kuyruktan çıkan f değeri, g_score'dan + h'den büyükse, ihmal et (eskimiş düğüm)
        // Not: Bu kontrol, priority queue'nun "decrease key" özelliğinin basit bir telafisidir.
        // g(u) + h(u) <= current_f kontrolü daha doğru olurdu, ama g(u) değerini PQ'dan alamıyoruz.
        // Basitlik için sadece hedef kontrolü yapalım.

        // Bitiş şehrine ulaşıldıysa
        if (u == end_city) {
            path_found = 1;
            break;
        }

        // u'nun komşularını gez
        Edge *edge = graph.adj[u];
        while (edge != NULL) {
            int v = edge->to;
            double weight = edge->distance; // u'dan v'ye olan gerçek maliyet

            // Yeni g_score (başlangıçtan v'ye olan maliyet)
            double tentative_g_score = g_score[u] + weight;

            // Relaksasyon: Daha kısa bir gerçek yol bulunduysa
            if (tentative_g_score < g_score[v]) {
                // Yeni g_score'u kaydet
                g_score[v] = tentative_g_score;
                parent[v] = u;

                // v'den bitişe olan sezgisel maliyet (h(v))
                double h_v = haversine_distance(graph.coords[v], graph.coords[end_city]);

                // Yeni f_score (öncelik)
                double f_score = g_score[v] + h_v;

                // Yeni f_score ile öncelik kuyruğuna ekle/güncelle
                // Not: Basit bir PQ kullanıldığı için, güncel değer yerine yeni düğüm eklenir.
                // Bu, performans kaybına neden olabilir (Dijkstra'da olduğu gibi).
                pq_push(&pq, v, f_score);
            }
            edge = edge->next;
        }
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Çıktı
    if (path_found) {
        printf("%.2f\n", g_score[end_city]); // Path uzunluğu (toplam mesafe)
        print_path(parent, end_city);         // Path üzerindeki düğümler
    } else {
        printf("-1.00\n-1\n"); // Yol bulunamadı
    }
    printf("A* Sure: %.6f s\n", time_spent);

    // Belleği serbest bırak
    free_pq(&pq);
    free(parent);
    free(g_score);
    printf("\n");
}


// --- Main Fonksiyonu ---

int main() {
    // Büyük veri kümeleri için G/Ç hızlandırması (isteğe bağlı, C++'da daha yaygın)
    // C'de `setvbuf(stdin, NULL, _IONBF, 0);` kullanılabilir, ancak standart okuma yeterlidir.

    // Girdi verilerini oku ve grafiği oluştur
    read_input();

    // 1) BFS algoritmasını çalıştır
    printf("1) BFS Algorithm\n");
    bfs_algorithm();

    // 2) Dijkstra algoritmasını çalıştır
    printf("2) Dijkstra Algorithm\n");
    dijkstra_algorithm();

    // 3) A* algoritmasını çalıştır
    printf("3) A* Algorithm\n");
    astar_algorithm();

    // Grafiğin belleğini serbest bırak
    free_graph(&graph);

    return 0;
}