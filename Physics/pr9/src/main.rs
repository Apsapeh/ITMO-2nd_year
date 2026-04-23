use plotters::prelude::*;

fn main() {
    let D = 15;
    let M = 1;

    let A = D as f64;
    let B = M as f64;
    let As = A / 8.0;
    let Bs = B / 8.0;

    let hx = 1.0 / 9.0;
    let hy = 1.0 / 7.0;

    let ls = |y: f64| B + A / 6.0 * y;

    // let mut plate = [
    //     [0.0, As * 1.0, As * 2.0, As * 3.0, As * 4.0, As * 5.0, As * 6.0, As * 7.0, ls(6.0)],
    //     [0.0, 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      ls(5.0)],
    //     [0.0, 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      ls(4.0)],
    //     [0.0, 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      ls(3.0)],
    //     [0.0, 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      ls(2.0)],
    //     [0.0, 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      ls(1.0)],
    //     [0.0, Bs * 1.0, Bs * 2.0, Bs * 3.0, Bs * 4.0, Bs * 5.0, Bs * 6.0, Bs * 7.0, ls(0.0)],
    // ];

    let mut plate = [[0.0; 9]; 7];

    let mut counter = 7;
    let mut sum = 0.0;
    for i in 1..8 {
        plate[0][i] = As * i as f64;
        plate[6][i] = Bs * i as f64;
        counter += 2;
        sum += As * i as f64;
        sum += Bs * i as f64;
    }

    for j in 0..7 {
        plate[j][8] = ls(6.0 - j as f64);
        sum += plate[j][8];
        counter += 1;
    }

    let avg = sum / counter as f64;

    for i in 1..8 {
        for j in 1..6 {
            plate[j][i] = avg;
        }
    }

    let mut prev_plate = plate;

    loop {
        let mut dt_sum = 0.0;
        for j in 1..6 {
            for i in 1..8 {
                let term_x = (prev_plate[j][i + 1] + prev_plate[j][i - 1]) / (hx * hx);
                let term_y = (prev_plate[j + 1][i] + prev_plate[j - 1][i]) / (hy * hy);
                let denominator = 2.0 / (hx * hx) + 2.0 / (hy * hy);

                plate[j][i] = (term_x + term_y) / denominator;
                dt_sum += f64::abs(prev_plate[j][i] - plate[j][i]);
            }
        }

        if dt_sum < 0.001 {
            break;
        }

        prev_plate = plate;
    }

    for j in 0..7 {
        print!("|");
        for i in 0..9 {
            print!(" {:^8.4} |", plate[j][i]);
        }
        println!();
    }

    draw(plate).unwrap();

    println!("Hello, world!");
}

fn draw(plate: [[f64; 9]; 7]) -> Result<(), Box<dyn std::error::Error>> {
    let root_drawing_area = BitMapBackend::new("plot-out/main.png", (900, 700)).into_drawing_area();
    let child_drawing_areas = root_drawing_area.split_evenly((7, 9));
    
    for (id, area) in child_drawing_areas.into_iter().enumerate() {
        let x = id % 9;
        let y = id / 9;
        area.fill(&spectral_color(plate[y][x])).unwrap();
        println!("id: {}", id);
    }

    root_drawing_area.present()?;
    Ok(())
}

fn spectral_color(v: f64) -> HSLColor {
    let t = (v / 16.0).clamp(0.0, 1.0);
    let t = 1.0 - t;
    let hue = t * 300.0;
    HSLColor(hue / 360.0, 1.0, 0.5)
}
