import { Component, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Router } from '@angular/router';
import { AuthService } from '../services/auth.service';
import { LoginRequest, RegisterRequest } from '../models/auth.model';

@Component({
  selector: 'app-login',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './login.html',
  styleUrl: './login.css'
})
export class LoginComponent {
  loginData: LoginRequest = { username: '', password: '' };
  registerData: RegisterRequest = { username: '', password: '' };
  isLoginMode = true;
  errorMessage = '';
  isLoading = false;

  constructor(private authService: AuthService, private router: Router, private cdRef: ChangeDetectorRef) {
    if (this.authService.isAuthenticated()) {
      this.router.navigate(['/main']);
    }
  }

  toggleMode(): void {
    this.isLoginMode = !this.isLoginMode;
    this.errorMessage = '';
  }

  onLogin(): void {
    if (!this.loginData.username || !this.loginData.password) {
      this.errorMessage = 'Заполните все поля';
      return;
    }

    this.isLoading = true;
    this.errorMessage = '';

    this.authService.login(this.loginData).subscribe({
      next: () => {
        this.router.navigate(['/main']);
      },
      error: () => {
        this.errorMessage = 'Неверный логин или пароль';
        this.isLoading = false;
        this.cdRef.detectChanges();
      }
    });
  }

  onRegister(): void {
    if (!this.registerData.username || !this.registerData.password) {
      this.errorMessage = 'Заполните все поля';
      return;
    }

    if (this.registerData.password.length < 3) {
      this.errorMessage = 'Пароль должен быть не менее 3 символов';
      return;
    }

    this.isLoading = true;
    this.errorMessage = '';

    this.authService.register(this.registerData).subscribe({
      next: () => {
        this.router.navigate(['/main']);
      },
      error: () => {
        this.errorMessage = 'Пользователь с таким именем уже существует';
        this.isLoading = false;
        this.cdRef.detectChanges();
      }
    });
  }
}
