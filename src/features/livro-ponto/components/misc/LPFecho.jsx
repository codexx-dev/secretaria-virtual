import { getMonthName, usePlural } from "../../../../utils";
import LPFooter from "./LPFooter";
import LPHead from "./LPHead";

function LPFecho(){
  const umeName = "UME Rural Monte Cabrão";
  const pageNum = 60;
  const numName = "sessenta";
  const refYear = 2025;
  const refMonth = 11;
  const lastMonth = (refMonth - 1 < 0)? 11 : refMonth - 1;
  const lastYear = (refMonth === 0)? refYear - 1 : refYear;

  return (
    <div className="size-full flex flex-col">
      <LPHead/>
      <div className="grow text-center text-lg flex flex-col">
        <h2 className="mt-5 text-xl font-semibold">{umeName.toUpperCase()}</h2>
        <h1 className="mt-80 text-5xl font-semibold underline">TERMO DE ENCERRAMENTO</h1>
        <p  className="text-balance leading-tight mt-10">
          {`Contém este volume ${pageNum} (${numName}) página${usePlural(pageNum > 1)} digitadas${usePlural(pageNum > 1)}, numerada${usePlural(pageNum > 1)} e por mim rubricada${usePlural(pageNum > 1)}, sendo-lhe destinado o uso indicado no termo de abertura.`}.
        </p>
        {/*precisa corrigir na primeira folha é o primeiro dia útil do ano*/}
        <span className="mt-5">{`Santos, 26 de ${getMonthName(lastMonth).toLocaleLowerCase()} de ${lastYear}.`}</span>
        <span className="mt-10">______________________________________</span>
      </div>
      <LPFooter/>
    </div>
  );
}

export default LPFecho;