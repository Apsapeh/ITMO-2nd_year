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
  username: string;
  id: number;
  x: number;
  y: number;
  r: number;
  isHit: boolean;
  dateTime: Date;
  executionTime: number;
}


export interface HitTableRequest {
  page: number;
  pageSize: number;
}

export interface HitTableResponse {
  results: HitResult[];
  totalCount: number;
  page: number;
  pageSize: number;
}