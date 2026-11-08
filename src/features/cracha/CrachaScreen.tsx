import { CrachaFooter } from "./CrachaFooter";
import { CrachaHeader } from "./CrachaHeader";
import { CrachaInfo, CrachaList } from "./CrachaList";

const nomes = [
  "Ana Souza",
  "Carlos Lima",
  "Beatriz Costa",
  "Diego Martins",
  "Fernanda Alves",
  "Gabriel Rocha",
  "Helena Ribeiro",
  "Igor Pereira",
  "Julia Carvalho",
  "Lucas Barbosa",
];
const salas = [
  "PRE A",
  "1º A",
  "2º B",
  "3º A",
  "4º C",
  "5º A",
  "6º B",
  "7º A",
  "8º C",
  "9º A",
];
const infoTest: CrachaInfo[] = Array.from({ length: 50 }, (_, i) => ({
  id: String(i + 1),
  nome: `${nomes[i % nomes.length]} ${i + 1}`,
  sala: salas[i % salas.length],
}));

export function CrachaScreen() {
  return (
    <div class="h-full flex flex-col">
      <CrachaHeader />
      <CrachaList crachasInfo={infoTest} />
      <CrachaFooter />
    </div>
  );
}
