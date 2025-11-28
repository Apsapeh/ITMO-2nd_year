export interface HitCheckRequest {
  x: number;
  y: number;
  r: number;
}

export interface HitCheckGraphRequest {
  x: number;
  y: number;
  r: number;
}

export interface HitResult {
  id: number;
  x: number;
  y: number;
  r: number;
  isHit: boolean;
  dateTime: Date;
  executionTime: number;
}

