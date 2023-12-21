function build_user_item_matrix() {
    global $wpdb;

    // Отримуємо всі унікальні користувачі та товари з бази даних
    $users = $wpdb->get_col("SELECT DISTINCT user_id FROM {$wpdb->prefix}user_item_interactions");
    $items = $wpdb->get_col("SELECT DISTINCT item_id FROM {$wpdb->prefix}user_item_interactions");

    $user_item_matrix = array();

    // Заповнюємо матрицю взаємодій
    foreach ($users as $user) {
        foreach ($items as $item) {
            // Отримуємо взаємодію користувача із товаром (наприклад, оцінка)
            $interaction = get_user_item_interaction($user, $item);

            // Додаємо взаємодію до матриці
            $user_item_matrix[$user][$item] = $interaction;
        }
    }

    return $user_item_matrix;
}

function get_user_item_interaction($user, $item) {
    global $wpdb;

    // Запит до бази даних для отримання взаємодії користувача із товаром
    $interaction = $wpdb->get_var(
        $wpdb->prepare(
            "SELECT interaction FROM {$wpdb->prefix}user_item_interactions WHERE user_id = %d AND item_id = %d",
            $user,
            $item
        )
    );

    return $interaction;
}

function predict_rating($user_id, $item_id) {
    global $wpdb;
    // Отримуємо список схожих користувачів
    $similar_users = get_similar_users($user_id);
    $weighted_sum = 0;
    $similarity_sum = 0;
    // Розрахунок прогнозованої оцінки на основі зваженої середньої
    foreach ($similar_users as $similar_user_id => $similarity) {
        // Отримуємо оцінку схожого користувача для даного товару
        $user_item_rating = get_user_item_interaction($similar_user_id, $item_id);
        if ($user_item_rating !== null) {
            // Додаємо внесок до зваженої суми
            $weighted_sum += $similarity * $user_item_rating;
            // Додаємо схожість до суми схожостей
            $similarity_sum += $similarity;
        }
    }
    // Робимо прогнозовану оцінку
    if ($similarity_sum > 0) {
        $predicted_rating = $weighted_sum / $similarity_sum;
        return $predicted_rating;
    } else {
        // Якщо схожих користувачів немає, повертаємо значення за замовчуванням або обробляємо інакше
        return get_default_rating();
    }
}
Функція для отримання списку схожих користувачів
function get_similar_users($user_id) {
    // Реалізація логіки отримання схожих користувачів
    // ...
    // Повертаємо асоціативний масив, де ключ - це ID схожого користувача, а значення - схожість
    return $similar_users;
}
// Функція для отримання оцінки користувача для певного товару
function get_user_item_interaction($user_id, $item_id) {
    // Реалізація логіки отримання оцінки користувача для товару
    // ...
    return $user_item_rating;
}
// Функція для повернення значення за замовчуванням в разі відсутності схожих користувачів
function get_default_rating() {
    // Реалізація логіки повернення значення за замовчуванням
    // ...
    return $default_rating;
}
function predict_user_item_interaction_with_filtering($user, $item, $user_item_matrix) {
    // Використовуємо прогнозування без фільтрації для отримання базового прогнозу
    $base_prediction = predict_user_item_interaction($user, $item, $user_item_matrix);
    // Фільтрація на основі популярності: виключаємо непопулярні товари
    $popularity_threshold = 5; // Наприклад, товари з рейтингом нижче 5 вважаються непопулярними
    $item_popularity = get_item_popularity($item, $user_item_matrix);
        if ($item_popularity < $popularity_threshold) {
        return 0; // Якщо товар непопулярний, повертаємо прогноз - 0
    }
    // Обмеження врахування неактивних користувачів: виключаємо користувачів з низькою активністю
    $activity_threshold = 10; // Наприклад, користувачі, які мають менше 10 взаємодій, вважаються неактивними
    $user_activity = count($user_item_matrix[$user]);
        if ($user_activity < $activity_threshold) {
        return 0; // Якщо користувач неактивний, повертаємо прогноз - 0
    }
    // Якщо всі фільтри пройдені, повертаємо базовий прогноз
    return $base_prediction;
}
function get_item_popularity($item, $user_item_matrix) {
    $total_interactions = 0;
    foreach ($user_item_matrix as $user => $items) {
        if (isset($items[$item])) {
            $total_interactions += $items[$item];
        }
    }
    return $total_interactions;
}
function get_user_item_matrix() {
    global $hardvare-store;
    $users = $hardvare-store->get_col("SELECT DISTINCT user_id FROM {$hardvare-store->prefix}user_item_interactions");
    $items = $hardvare-store->get_col("SELECT DISTINCT item_id FROM {$hardvare-store->prefix}user_item_interactions");
    $user_item_matrix = array();
    foreach ($users as $user) {
        foreach ($items as $item) {
            $interaction = get_user_item_interaction($user, $item);
            $user_item_matrix[$user][$item] = $interaction;
        }
    }
    return $user_item_matrix;
}
// Отримання взаємодії користувача із товаром
function get_user_item_interaction($user, $item) {
    global $hardvare-store;
    $interaction = $hardvare-store->get_var(
        $hardvare-store->prepare(
            "SELECT interaction FROM {$hardvare-store->prefix}user_item_interactions WHERE user_id = %d AND item_id = %d",
            $user,
            $item
        )
    );
    return $interaction;
}
// Визначення косинусної схожості між двома користувачами
function calculate_cosine_similarity($user1, $user2, $user_item_matrix) {
    $dot_product = 0;
    $magnitude_user1 = 0;
    $magnitude_user2 = 0;
    foreach ($user_item_matrix[$user1] as $item => $interaction_user1) {
        if (isset($user_item_matrix[$user2][$item])) {
            $interaction_user2 = $user_item_matrix[$user2][$item];
            $dot_product += $interaction_user1 * $interaction_user2;
        }
        $magnitude_user1 += pow($interaction_user1, 2);
    }
    foreach ($user_item_matrix[$user2] as $item => $interaction_user2) {
        $magnitude_user2 += pow($interaction_user2, 2);
    }
    $magnitude_user1 = sqrt($magnitude_user1);
    $magnitude_user2 = sqrt($magnitude_user2);
    if ($magnitude_user1 == 0 || $magnitude_user2 == 0) {
        return 0; // Якщо вектори мають нульову довжину, схожість буде нульовою.
    }
    $similarity = $dot_product / ($magnitude_user1 * $magnitude_user2);
    return $similarity;
}
// Приклад використання
$user_item_matrix = get_user_item_matrix();
$user1 = 1;
$user2 = 2;
$similarity = calculate_cosine_similarity($user1, $user2, $user_item_matrix);
echo "Cosine Similarity between User $user1 and User $user2: $similarity";


<form action="process_interactions.php" method="post">
    <label for="item1">Item 1:</label>
    <input type="number" name="item1" min="1" max="5" required>
    <!-- Додамо інші елементи для інших товарів -->
    <button type="submit">Submit</button>
</form>
// process_interactions.php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $user_id = get_current_user_id(); // Отримайте ID поточного користувача (припускаючи, що це WordPress)
        $item1_rating = $_POST["item1"];
    // Отримайте рейтинги інших товарів з POST-запиту
    // Збереження взаємодій в базу даних
    global $hardvare-store;
    $table_name = $hardvare-store->prefix . 'user_item_interactions';
    $hardvare-store->insert(
        $table_name,
        array(
            'user_id' => $user_id,
            'item_id' => 1, // ID товару, що відповідає item1
            'interaction' => $item1_rating,
        )
    );
    // Додамо аналогічні операції для інших товарів
    // Перенаправлення користувача або виведення повідомлення про успішне оцінювання
}
//Отримання рекомендацій для користувача
function get_recommendations_for_user($user_id) {
    $user_item_matrix = get_user_item_matrix(); // Функція отримання матриці взаємодій користувачів із товарами
    // Визначення схожості між користувачами
    $similar_users = find_similar_users($user_id, $user_item_matrix);
    // Обчислення рекомендацій для користувача
    $recommendations = calculate_recommendations($user_id, $similar_users, $user_item_matrix);
    // Фільтрація та покращення результатів
    $filtered_recommendations = apply_filters($recommendations, $user_id);
    return $filtered_recommendations;
}
//Знаходження схожих користувачів
function find_similar_users($user_id, $user_item_matrix) {
    $similar_users = array();
    // Логіка для визначення схожості між користувачами
    // ...
    return $similar_users;
}
//Обчислення рекомендацій для користувача
function calculate_recommendations($user_id, $similar_users, $user_item_matrix) {
    $recommendations = array();
    // Логіка для обчислення рекомендацій на основі схожості між користувачами
    // ...
    return $recommendations;
}
// Фільтрація та покращення результатів
function apply_filters($recommendations, $user_id) {
    $filtered_recommendations = array();
    // Логіка для фільтрації та покращення результатів
    // ...
    return $filtered_recommendations;
}
// Оцінка точності рекомендаційної системи
function evaluate_recommendation_system() {
    $test_data = get_test_data(); // Отримання тестових даних для оцінки
    $predicted_ratings = array();

    foreach ($test_data as $user_id => $items) {
        foreach ($items as $item_id => $actual_rating) {
            // Отримання передбаченого рейтингу для користувача та товару
            $predicted_rating = get_predicted_rating($user_id, $item_id);

            // Збереження передбаченого рейтингу для подальшої оцінки
            $predicted_ratings[$user_id][$item_id] = $predicted_rating;
        }
    }
    // Оцінка точності, наприклад, за допомогою середньоквадратичної помилки (RMSE)
    $rmse = calculate_rmse($test_data, $predicted_ratings);
    // Покращення продуктивності (наприклад, оптимізація алгоритмів)
    optimize_recommendation_system();
    return $rmse;
}
// Отримання тестових даних для оцінки
function get_test_data() {
    // Логіка для отримання тестових даних
    // ...
    return $test_data;
}
// Отримання передбаченого рейтингу для користувача та товару
function get_predicted_rating($user_id, $item_id) {
    // Логіка для отримання передбаченого рейтингу
    // ...
    return $predicted_rating;
}

// Розрахунок середньоквадратичної помилки (RMSE)
function calculate_rmse($actual_ratings, $predicted_ratings) {
    $sum_squared_error = 0;
    $total_ratings = 0;
    foreach ($actual_ratings as $user_id => $items) {
        foreach ($items as $item_id => $actual_rating) {
            $predicted_rating = $predicted_ratings[$user_id][$item_id];
            $error = $actual_rating - $predicted_rating;

            $sum_squared_error += pow($error, 2);
            $total_ratings++;
        }
    }
    $mean_squared_error = $sum_squared_error / $total_ratings;
    $rmse = sqrt($mean_squared_error);
    return $rmse;
}

// Покращення продуктивності рекомендаційної системи
function optimize_recommendation_system() {
    // Логіка для оптимізації алгоритмів рекомендаційної системи
    // ...
}

системи в плагіні WordPress:
// Система збору та обробки даних
class RecommendationDataProcessor {
    // Отримання та зберігання взаємодій користувачів із товарами
    public static function collect_user_item_interactions($user_id, $item_id, $interaction) {
        global $hardvare-store;
        $table_name = $hardvare-store->prefix . 'user_item_interactions';

        // Перевірка наявності запису для уникнення дублікатів
        $existing_interaction = $hardvare-store->get_var(
            $hardvare-store->prepare(
                "SELECT interaction FROM $table_name WHERE user_id = %d AND item_id = %d",
                $user_id,
                $item_id
            )
        );
       if ($existing_interaction !== null) {
            // Якщо запис існує, оновити взаємодію
            $hardvare-store->update(
                $table_name,
                array('interaction' => $interaction),
                array('user_id' => $user_id, 'item_id' => $item_id)
            );
        } else {
            // Якщо запис не існує, вставити новий запис
            $hardvare-store->insert(
                $table_name,
                array('user_id' => $user_id, 'item_id' => $item_id, 'interaction' => $interaction),
                array('%d', '%d', '%d')
            );
        }
    }
    // Аналіз даних та побудова матриці взаємодій
    public static function build_user_item_matrix() {
        global $hardvare-store;
        $table_name = $hardvare-store->prefix . 'user_item_interactions';

        $users = $hardvare-store->get_col("SELECT DISTINCT user_id FROM $table_name");
        $items = $hardvare-store->get_col("SELECT DISTINCT item_id FROM $table_name");
        $user_item_matrix = array();
        foreach ($users as $user) {
            foreach ($items as $item) {
                $interaction = self::get_user_item_interaction($user, $item);
                $user_item_matrix[$user][$item] = $interaction;
            }
        }
        return $user_item_matrix;
    }

    // Отримання взаємодії користувача із товаром
    private static function get_user_item_interaction($user_id, $item_id) {
        global $hardvare-store;
        $table_name = $hardvare-store->prefix . 'user_item_interactions';

        $interaction = $hardvare-store->get_var(
            $hardvare-store->prepare(
              "SELECT interaction FROM $table_name WHERE user_id = %d AND item_id = %d",
                $user_id,
                $item_id
            )
        );

        return $interaction;
    }
}

// Використання системи збору та обробки даних
// Приклад збору взаємодії користувача 1 із товаром 5 з оцінкою 4
$user_id = 1;
$item_id = 5;
$interaction = 4;

RecommendationDataProcessor::collect_user_item_interactions($user_id, 
$item_id, $interaction);

// Приклад побудови матриці взаємодій
$user_item_matrix = RecommendationDataProcessor::build_user_item_matrix();


class RecommendationSystem {
    // Матриця взаємодій користувач-товар
    private $userItemMatrix;
    // Конструктор
    public function __construct() {
        // Ініціалізація матриці взаємодій при створенні об'єкта
        $this->userItemMatrix = $this->buildUserItemMatrix();
    }
    // Метод для побудови матриці взаємодій
    private function buildUserItemMatrix() {
        $dataProcessor = new RecommendationDataProcessor();
        return $dataProcessor->buildUserItemMatrix();
    }
    // Метод для знаходження схожих користувачів
    private function findSimilarUsers($userId) {
        // Логіка для визначення схожості між користувачами
        // ...
        return $similarUsers;
    }
    // Метод для прогнозування рейтингу
    private function predictRating($userId, $itemId, $similarUsers) {
        // Логіка для прогнозування рейтингу
        // ...
        return $predictedRating;
    }
    // Метод для фільтрації та покращення результатів
    private function applyFilters($recommendations, $userId) {
        // Логіка для фільтрації та покращення результатів
        // ...
        return $filteredRecommendations;
    }
    // Метод для оцінки та покращення продуктивності
    public function evaluateAndOptimize() {
        // Логіка для оцінки та покращення продуктивності
        // ...
    }
    // Метод для отримання рекомендацій для користувача
    public function getRecommendations($userId) {
        $similarUsers = $this->findSimilarUsers($userId);
        
        // Отримання списку всіх товарів
        $allItems = array_keys($this->userItemMatrix[$userId]);

        $recommendations = array();

        foreach ($allItems as $itemId) {
            // Пропускаємо товари, які користувач вже взаємодіяв
            if (isset($this->userItemMatrix[$userId][$itemId])) {
                continue;
            }

            // Прогнозуємо рейтинг для непереглянутих товарів
            $predictedRating = $this->predictRating($userId, $itemId, $similarUsers);

            // Додаємо товар із прогнозованим рейтингом до рекомендацій
            $recommendations[$itemId] = $predictedRating;
        }

        // Фільтрація та покращення результатів
        $filteredRecommendations = $this->applyFilters($recommendations, $userId);

        return $filteredRecommendations;
    }
}

// Використання рекомендаційної системи
$recommendationSystem = new RecommendationSystem();
$userId = 1;
$recommendations = $recommendationSystem->getRecommendations($userId);



// Клас для обробки рекомендацій
class RecommendationHandler {
    // Отримання рекомендацій для користувача
    public static function getRecommendationsForUser($userId) {
        $recommendationSystem = new RecommendationSystem();
        return $recommendationSystem->getRecommendations($userId);
    }
}
// Функція для виведення рекомендацій в шаблоні
function displayRecommendations() {
    $currentUserId = get_current_user_id(); // Отримуємо ID поточного користувача
    $recommendations = RecommendationHandler::getRecommendationsForUser($currentUserId);
    // Виведення рекомендацій
    echo '<h2>Рекомендації для вас:</h2>';
    echo '<ul>';
    foreach ($recommendations as $itemId => $rating) {
        // Отримання інформації про товар за ID $itemId та виведення його
        $product = wc_get_product($itemId);
        echo '<li>' . $product->get_name() . ' - Рейтинг: ' . $rating . '</li>';
    }
    echo '</ul>';
}
// Виклик функції в шаблоні або блоку
displayRecommendations();


// Код для збору та обробки даних для фільтрації на основі контенту
function collect_content_data() {
    // Ваш код для збору та обробки даних про товари
}
add_action('woocommerce_before_single_product', 'collect_content_data');
// Код для збору та обробки даних для коллаборативного фільтрування
function collect_collaborative_data() {
    // Ваш код для збору та обробки даних взаємодії користувачів і товарів
}
// Реєстрація хуків для спрацьовування при завантаженні сторінок
add_action('template_redirect', 'collect_collaborative_data');
// Код для побудови профілю користувача
function build_user_profile($user_id) {
    // Ваш код для побудови профілю користувача
}
// Код для рекомендацій на основі фільтрації на основі контенту
function content_based_recommendations($user_id) {
    // Ваш код для генерації рекомендацій на основі фільтрації на основі контенту
}
// Код для рекомендацій на основі коллаборативного фільтрування
function collaborative_filtering_recommendations($user_id) {
    // Ваш код для генерації рекомендацій на основі коллаборативного фільтрування
}
// Код для гібридної рекомендаційної системи
function hybrid_recommendations($user_id) {
    $content_based = content_based_recommendations($user_id);
    $collaborative = collaborative_filtering_recommendations($user_id);

    // Комбінуємо рекомендації, наприклад, за допомогою зваженого середнього
    $hybrid_recommendations = array_merge($content_based, $collaborative);

    return $hybrid_recommendations;
}

