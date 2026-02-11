import LPHead from "./LPHead";

function LPIndice(){
  const umeName = "ume rural monte cabrão";
  const title1 = "índice - funcionários - estatutários";
  const title2 = "projetos";
  const maxLength = (title2)? 30 : 32;
  const pageNum = 1;

  return(
    <div className="size-full flex flex-col">

      <LPHead/>

      <table className='table-auto border-collapse mt-8'>
        <colgroup>
          <col className="w-0"/>
          <col className="w-auto"/>
          <col className="w-0"/>
          <col className="w-[22%]"/>
        </colgroup>
        <tr className="*:border text-center font-semibold">
          <td>&nbsp;</td>
          <td>{umeName.toUpperCase()}</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>
        <tr className="*:border">
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>
        <tr className="*:border text-center font-semibold">
          <td>&nbsp;</td>
          <td>{title1.toUpperCase()}</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>
        <tr className="*:border">
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>
        <tr className="*:border *:px-1 text-center font-semibold">
          <td>PAG.</td>
          <td>NOME DO SERVIDOR</td>
          <td>REGISTRO</td>
          <td>CARGO</td>
        </tr>
        
        {Array.from({length: maxLength - 5}).map((_, idx)=>{
          return (
          <tr className="*:border *:px-1 text-center" key={idx}>
            <td>{String(idx + 1).padStart(2, '0')}</td>
            <td className="text-left">ÁLVARO ESTEVES MATOS</td>
            <td>40.558-9</td>
            <td>Secretário de U.E.</td>
          </tr>
        );})}

        <tr>
          <td colSpan={4}>&nbsp;</td>
        </tr>
        <tr className="*:border text-center font-semibold">
          <td>&nbsp;</td>
          <td>{title2.toUpperCase()}</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
        </tr>

        {Array.from({length: 5}).map((_, idx)=>{
          return (
          <tr className="*:border *:px-1 text-center" key={idx}>
            <td>{String(idx + 1).padStart(2, '0')}</td>
            <td className="text-left">ÁLVARO ESTEVES MATOS</td>
            <td>40.558-9</td>
            <td>Secretário de U.E.</td>
          </tr>
        );})}

      </table>
    </div>
  );
}

export default LPIndice;

