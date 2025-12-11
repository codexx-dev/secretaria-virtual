import LPFooter from "./LPFooter";
import LPHead from "./LPHead";
import { getMonthName } from "../../../utils";

function LPCapa(){
  const umeName = "ume rural monte cabrão";
  const title = "registro de ponto dos funcionários"
  const refYear = 2025;
  const refMonth = 11;
  const lastMonth = (refMonth - 1 < 0)? 11 : refMonth - 1;

  return (
    <div className="size-full flex flex-col">
      <LPHead/>
      <div className="*:text-center grow flex flex-col">
        <h2 className="mt-5 text-xl font-semibold">{umeName.toUpperCase()}</h2>
        <h1 className="text-5xl leading-30 mt-40">{title.toUpperCase()}</h1>
        <h1 className="text-4xl mt-40">PERÍODO:</h1>
        <h1 className="text-4xl">{`26 DE ${getMonthName(lastMonth).toUpperCase()} A 25 DE ${getMonthName(refMonth).toUpperCase()}`}</h1>
        <h1 className="text-4xl mt-20">{refYear}</h1>
      </div>
      <LPFooter/>
    </div>
  );
}

export default LPCapa;