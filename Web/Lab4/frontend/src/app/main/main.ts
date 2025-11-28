import { Component, OnInit, AfterViewInit, ElementRef, ViewChild, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Router } from '@angular/router';
import { AuthService } from '../services/auth.service';
import { HitService } from '../services/hit.service';
import { HitResult } from '../models/hit.model';

@Component({
  selector: 'app-main',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './main.html',
  styleUrl: './main.css'
})
export class MainComponent implements OnInit, AfterViewInit {
  @ViewChild('canvas', { static: false }) canvasRef!: ElementRef<HTMLCanvasElement>;
  
  xValues = [-3, -2, -1, 0, 1, 2, 3, 4, 5];
  rValues = [-3, -2, -1, 0, 1, 2, 3, 4, 5];
  
  selectedX: number | null = null;
  y: number = 0;
  selectedR: number = 1;
  
  results: HitResult[] = [];
  currentPage = 0;
  pageSize = 30;
  totalResults = 0;
  isLoading = false;
  isClearing = false;
  errorMessage = '';
  warnMessage = '';
  
  username = '';

  constructor(
    private authService: AuthService,
    private hitService: HitService,
    private router: Router,
    private cdRef: ChangeDetectorRef
  ) {
    this.username = this.authService.getUsername() || '';
    if (!this.authService.isAuthenticated()) {
      this.router.navigate(['/login']);
    }
  }

  ngOnInit(): void {
    this.loadResults();
  }

  ngAfterViewInit(): void {
    setTimeout(() => this.drawGraph(), 100);
  }

  loadResults(): void {
    this.hitService.getResults({page: this.currentPage, pageSize: this.pageSize}).subscribe({
      next: (results) => {
        this.results = results.results;
        this.totalResults = results.totalCount;

        this.drawGraph();
        this.cdRef.detectChanges();
      },
      error: () => {
        this.errorMessage = 'Ошибка загрузки результатов';
      }
    });
  }

  prevPage(): void {
    if (this.currentPage > 0) {
      this.currentPage--;
      this.loadResults();
    }
  }

  nextPage(): void {
    if ((this.currentPage + 1) * this.pageSize < this.totalResults) {
      this.currentPage++;
      this.loadResults();
    }
  }

  onPageSizeChange(): void {
    let newSize = parseInt(this.pageSize as any, 10);
    
    if (isNaN(newSize) || newSize < 1) { 
        newSize = 1;
    }
    
    if (newSize > 1000) {
        newSize = 1000;
    }

    console.log(this.pageSize);
    
    this.pageSize = newSize; 

    console.log(this.pageSize);

    this.currentPage = 0;
    this.loadResults();
  }

  selectX(value: number): void {
    this.selectedX = value;
  }

  selectR(value: number): void {
    this.selectedR = value;
    if (value <= 0) {
      this.warnMessage = 'R не может быть равен нулю или отрицательным';
      return;
    };
    this.warnMessage = '';
    this.drawGraph();
    this.loadResults();
  }

  validateY(): void {
    if (this.y < -5) this.y = -5;
    if (this.y > 5) this.y = 5;
  }

  checkHit(): void {
    if (this.selectedX === null) {
      this.errorMessage = 'Выберите значение X';
      return;
    }

    if (this.y < -5 || this.y > 5) {
      this.errorMessage = 'Y должен быть в диапазоне от -5 до 5';
      return;
    }

    if (this.selectedR <= 0) {
      this.errorMessage = 'R не может быть равен нулю или отрицательным';
      return;
    }

    this.isLoading = true;
    this.errorMessage = '';

    this.hitService.checkHit({
      x: this.selectedX,
      y: this.y,
      r: this.selectedR
    }).subscribe({
      next: () => {
        this.loadResults();
        this.isLoading = false;
        this.cdRef.detectChanges();
      },
      error: () => {
        this.errorMessage = 'Ошибка проверки точки';
        this.isLoading = false;
        this.cdRef.detectChanges();
      }
    });
  }

  onCanvasClick(event: MouseEvent): void {
    if (this.selectedR === 0) {
      this.errorMessage = 'Выберите значение R';
      return;
    }

    const canvas = this.canvasRef.nativeElement;
    const rect = canvas.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    // Convert canvas coordinates to graph coordinates
    const graphX = ((x - 200) / 120) * this.selectedR;
    const graphY = -((y - 200) / 120) * this.selectedR;

    if (this.selectedR <= 0) {
      this.errorMessage = 'R не может быть равен нулю или отрицательным';
      return;
    }

    this.selectedX = graphX;
    this.y = graphY;

    this.isLoading = true;
    this.errorMessage = '';

    this.hitService.checkHitGraph({
      x: this.selectedX,
      y: this.y,
      r: this.selectedR
    }).subscribe({
      next: () => {
        this.currentPage = 0;
        this.loadResults();
        this.isLoading = false;
        this.cdRef.detectChanges();
      },
      error: () => {
        this.errorMessage = 'Ошибка проверки точки';
        this.isLoading = false;
        this.cdRef.detectChanges();
      }
    });
  }

  clearResults(): void {
    this.isClearing = true;
    this.errorMessage = '';

    this.hitService.clearResults().subscribe({
      next: () => {
        this.isClearing = false;
        this.currentPage = 0;
        this.loadResults();
      },
      error: () => {
        this.errorMessage = 'Ошибка очистки результатов';
        this.isClearing = false;
        this.cdRef.detectChanges();
      }
    });
  }

  drawGraph(): void {
    if (this.selectedR <= 0) return;

    const canvas = this.canvasRef?.nativeElement;
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    const width = 400;
    const height = 400;
    canvas.width = width;
    canvas.height = height;

    const centerX = width / 2;
    const centerY = height / 2;
    let scale = 120;

    // Clear canvas
    ctx.clearRect(0, 0, width, height);
    ctx.fillStyle = '#ffffff';
    ctx.fillRect(0, 0, width, height);

    // Draw coordinate system
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 2;

    // Axes
    ctx.beginPath();
    ctx.moveTo(0, centerY);
    ctx.lineTo(width, centerY);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(centerX, 0);
    ctx.lineTo(centerX, height);
    ctx.stroke();

    // Draw area
    ctx.fillStyle = 'rgba(100, 150, 255, 0.5)';
    ctx.strokeStyle = '#0066cc';
    ctx.lineWidth = 2;
    
    // Rectangle
    ctx.beginPath();
    const rectX1 = centerX - scale;
    const rectX2 = centerX;
    const rectY1 = centerY - scale * 0.5;
    const rectY2 = centerY;
    ctx.rect(rectX1, rectY1, scale, scale * 0.5);
    ctx.fill();
    ctx.stroke();
    
    // Triangle
    ctx.beginPath();
    ctx.moveTo(centerX, centerY);
    ctx.lineTo(centerX + scale * 0.5, centerY);
    ctx.lineTo(centerX, centerY - scale * 0.5);
    ctx.closePath();
    ctx.fill();
    ctx.stroke();
    
    // Semicircle
    ctx.beginPath();
    ctx.arc(centerX, centerY, scale * 0.5, 0, Math.PI / 2, false);
    ctx.lineTo(centerX, centerY);
    ctx.closePath();
    ctx.fill();
    ctx.stroke();

    // Draw axes labels
    ctx.fillStyle = '#000';
    ctx.font = '12px Arial';
    ctx.textAlign = 'center';

    const labels = [-2, -1.5, -1, -0.5, 0.5, 1, 1.5, 2];
    labels.forEach(val => {
      if (val !== 0) {
        const x = centerX + val * scale;
        const y = centerY - val * scale;
        ctx.fillText((val * this.selectedR).toString(), x, centerY + 15);
        ctx.fillText((val * this.selectedR).toString(), centerX + 15, y);

        ctx.strokeStyle = '#ccc';
        ctx.lineWidth = 1;

        ctx.beginPath();
        ctx.moveTo(x, 0);
        ctx.lineTo(x, height);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, y);
        ctx.lineTo(width, y);
        ctx.stroke();
      }
    });

    // Draw existing points
    this.results.forEach(result => {
      if (result.r === this.selectedR) {
        const absR = Math.abs(result.r);
        const x = centerX + (result.x / absR) * scale;
        const y = centerY - (result.y / absR) * scale;
        
        ctx.fillStyle = result.isHit ? '#00ff00' : '#ff0000';
        ctx.beginPath();
        ctx.arc(x, y, 4, 0, 2 * Math.PI);
        ctx.fill();
      }
    });
  }

  logout(): void {
    this.authService.logout();
  }

  formatDate(date: Date): string {
    return new Date(date).toLocaleString('ru-RU');
  }
}
