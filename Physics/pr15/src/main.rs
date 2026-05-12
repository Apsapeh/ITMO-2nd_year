use rand::{Rng, RngExt, SeedableRng};
use rand::rngs::StdRng;

/*
    Вариант 3

    Стратегия по рождению мальчика.
    Пока не родится мальчик продолжают рожать детей.
    Сколько в среднем будет детей в семье?
    Вероятность 50/50

    Решить методом Монте-Карло
*/



fn main() {
    let experiments = 1_000_000;
    let mut total_children = 0;

    let mut rng = StdRng::seed_from_u64(42);

    for _ in 0..experiments {
        let mut children = 0;

        loop {
            children += 1;

            let boy = rng.random_bool(0.5);

            if boy {
                break;
            }
        }

        total_children += children;
    }

    let average = total_children as f64 / experiments as f64;

    println!("Среднее число детей: {}", average);
}
