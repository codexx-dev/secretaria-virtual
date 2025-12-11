export function usePlural(condition){
  return (condition)? 's' : '';
}

export function formatNN(number, size=2){
  return String(number).padStart(size, '0');
}

export function isoTime(year, month, day){
  return `${String(year)}-${String(month).padStart(2, '0')}-${String(day).padStart(2, '0')}`; 
}

export function has31Days(month, year){
  return new Date(year, month -1, 0).getDate() === 31;
}

export function getWeekday(day, month, year) {
  return new Date(year, month - 1, day).getDay();
}

export function getNumberName(num){
  const numberNames = [
    'zero', 'um', 'dois', 'três', 'quatro', 'cinco', 'seis', 'sete', 'oito', 'nove',
    'dez', 'onze', 'doze', 'treze', 'quatorze', 'quinze', 'dezeseis', 'dezesete', 'dezoito', 'dezenove',
    'vinte', 'trinta', 'quarenta', 'cinquenta', 'sessenta', 'setenta', 'oitenta', 'noventa',
    'cem', 'duzentos', 'trezentos', 'quatrocentos', 'quinhentos', 'seiscentos', 'setecentos', 'oitocentos', 'novecentos', 'mil'
  ];
  const nameIdx = (num % 10);
  return numberNames[nameIdx];
}

export function getWeekName(weekDay){
    const d = [
    "Domingo",
    "Segunda",
    "Terça",
    "Quarta",
    "Quinta",
    "Sexta",
    "Sábado"
  ];
  return d[weekDay];
}

export function getMonthName(month){
    const m = [
    "Janeiro",
    "Fevereiro",
    "Março",
    "Abril",
    "Maio",
    "Junho",
    "Julho",
    "Agosto",
    "Setembro",
    "Outubro",
    "Novembro",
    "Dezembro"
  ];
  return m[(month - 1) % 12];
}

export function pickRandom(arr){
  return arr[Math.floor(Math.random() * arr.length)];
}