  DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys[VK_SPACE])
    VG4_AnimAddUnit(VG4_UnitCreateBall());
  if (Ani->Keys['C'])
    VG4_AnimAddUnit(VG4_UnitCreateCube());
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    VG4_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    VG4_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateX(59 * Ani->Mdy * Ani->GlobalDeltaTime));
  }

  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(59 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr43(Uni->Pos, MatrRotateY(-59 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->DeltaTime * 0.1) / r);

  VG4_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
