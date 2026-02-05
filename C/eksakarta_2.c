#include <stdio.h>
#include <math.h>
#include <float.h>

#define MAXN 20
#define INF DBL_MAX

double dist[MAXN][MAXN];
double dp[1 << MAXN][MAXN];
int n;

double euclidean_dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int main() {
    if (scanf("%d", &n) != 1) return 1;

    double x[MAXN], y[MAXN];
    for (int i = 0; i < n; ++i) {
        scanf("%lf %lf", &x[i], &y[i]);
    }

    // Предварительный расчет всех попарных расстояний
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                dist[i][j] = 0.0;
            } else {
                dist[i][j] = euclidean_dist(x[i], y[i], x[j], y[j]);
            }
        }
    }

    // Инициализация DP таблицы
    // dp[mask][i] = минимальный путь, посещающий города в mask и заканчивающийся в i
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            dp[mask][i] = INF;
        }
    }

    // Базовый случай: начинаем с города 0, маска {0}
    dp[1 << 0][0] = 0.0;

    // Итерация по всем маскам (подмножествам городов)
    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int last = 0; last < n; ++last) {
            // Если город 'last' не входит в текущую маску или путь до него невозможен, пропускаем
            if (!(mask & (1 << last)) || dp[mask][last] == INF) continue;

            // Пытаемся перейти в следующий город 'next'
            for (int next = 0; next < n; ++next) {
                // Если город 'next' уже посещен (входит в mask), пропускаем
                if (!(mask & (1 << next))) {
                    int next_mask = mask | (1 << next);
                    // Обновляем минимальный путь до next_mask, заканчивающийся в next
                    dp[next_mask][next] = fmin(dp[next_mask][next], dp[mask][last] + dist[last][next]);
                }
            }
        }
    }

    // Финальный шаг: ищем минимальный цикл, возвращаясь в город 0 из любого конечного города i
    double min_cycle_len = INF;
    int full_mask = (1 << n) - 1;

    for (int i = 0; i < n; ++i) {
        min_cycle_len = fmin(min_cycle_len, dp[full_mask][i] + dist[i][0]);
    }

    // Вывод результата с требуемой точностью
    printf("%.6lf\n", min_cycle_len);

    return 0;
}