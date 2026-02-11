function LPFooter(){
  const adress = "Rodovia Piaçaguera-Guarujá, KM 33, Bairro do Monte Cabrão Cidade de Santos-SP CEP 11.220-000";
  const tel = "(13) 3352-4991";
  const email = "montecabrao-seduc@santos.sp.gov.br";

  return (
    <div className="w-full text-sm text-center">
      <span>{adress}</span>
      <div className="flex flex-row justify-center gap-10">
        <span>Tel.: {tel}</span>
        <span>E-mail: <span className="underline">{email}</span></span>
      </div>
    </div>
  );
}

export default LPFooter;